/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2025 Open Risk (https://www.openriskmanagement.com)+
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "model_component.h"

/**
 * Correlation Matrix Model Component. Enabling the modeling of correlation between entities
 */
class CorrelationMatrix : public ModelComponent {
public:

    CorrelationMatrix(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Correlation Matrix"){
        std::string fragment = "CorrelationMatrix";
        Initialize(fragment, W, M, logstream);
    }

    ~CorrelationMatrix() override = default;

    void Deserialize(const std::string &json, Model &M, Simulation &W, Poco::LogStream &logstream) {

        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;
        Poco::JSON::Array::Ptr JsonArray;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing correlation matrix data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
            JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
            JsonArray = JsonObject->getArray("correlation_matrix");
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to parse correlation matrix object. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        try {
            m_CorrelationDataMode = JsonObject->getValue<int>("Correlation_data_mode");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading correlation data mode")) << std::endl;
            abort();
        }

        int found_factors = JsonArray->size();
        if (found_factors != m_Factors) {
            logstream.information() << BOLD(FYEL("> Warning: Matrix Size Not Consistent")) << std::endl;
        }
        try {
            m_CorrelationMethod = JsonObject->getValue<int>("CorrelationMethod");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading Correlation Method")) << std::endl;
            abort();
        }
        if (m_CorrelationMethod == 1) {
            // Reading multi-factor correlation structure
            try {
                m_Factors = JsonObject->getValue<int>("GetFactors");
            } catch (...) {
                logstream.error() << BOLD(FRED("> ERROR: Problem reading factor number")) << std::endl;
                abort();
            }
        } else {
            m_Factors = 1;
        }

        if (m_CorrelationMethod == 0) {
            try {
                m_SingleFactorCorrelation = JsonObject->getValue<double>("Correlation");
            } catch (...) {
                logstream.error() << BOLD(FRED("> ERROR: Problem reading correlation")) << std::endl;
                abort();
            }
        } else if (m_CorrelationMethod == 1) {
            try {
                m_Factors = JsonObject->getValue<int>("Factors");
                rho.resize(m_Factors, m_Factors);
            } catch (...) {
                logstream.error() << BOLD(FRED("> ERROR: Problem reading factor number")) << std::endl;
                abort();
            }
        }

        try {
            Poco::JSON::Array::Ptr Row;
            for (int i = 0; i < m_Factors; i++) {
                Row = JsonArray->getArray(i);
                for (int j = 0; j < m_Factors; j++) {
                    double corr_value = Row->get(j);
                    rho(i, j) = corr_value;
                }
            }
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to resolve correlation matrix values. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing loadings data from json." << std::endl;
        }

        if (m_CorrelationMethod == 1) {
            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 3 | Decomposing Correlation Matrix ." << std::endl;
            }
            this->DecomposeRho();
        }

        if (LOG_LEVEL >= 1) {
            logstream.information() << ">> 3 | Correlation Matrix: " << std::endl;
            if (LOG_LEVEL >= 3) {
                for (int i = 0; i < m_Factors; i++) {
                    for (int j = 0; j < m_Factors; j++)
                        logstream.information() << rho(i, j) << "\t";
                    logstream.information() << std::endl;
                }
                logstream.information() << std::endl;
            }

            logstream.information() << ">> 3 | Cholesky Matrix: " << std::endl;
            if (LOG_LEVEL >= 3) {
                for (int i = 0; i < m_Factors; i++) {
                    for (int j = 0; j < m_Factors; j++)
                        logstream.information() << L(i, j) << "\t";
                    logstream.information() << std::endl;
                }
                logstream.information() << std::endl;
            }

        }

    }

    void CorrFromAPI(std::string APItoken, Model &M, Simulation &W, Poco::LogStream &logstream) {
        try {

            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 3 | Fetching correlation data via API " << std::endl;
            }

            Poco::URI uri(W.CorrelationURL());

#if SSL_MODE == 0
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
#elif SSL_MODE == 1
            const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") );
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
#elif SSL_MODE==2
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
#endif

            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            std::string jwt_string = "JWT " + APItoken;
            req.add("Authorization", jwt_string);
            session.sendRequest(req);

            Poco::Net::HTTPResponse res;
            std::istream &is = session.receiveResponse(res);
            std::stringstream correlation_data;
            Poco::StreamCopier::copyStream(is, correlation_data);
            std::string json = correlation_data.str();

            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 3 | Correlation Data Server Response " << std::endl;
                logstream.information() << ">> 3 | " << res.getStatus() << " " << res.getReason() << std::endl;
                if (LOG_LEVEL >= 3) {
                    logstream.information() << json << std::endl;
                }
            }

            this->Deserialize(json, M, W, logstream);

        } catch (Poco::Exception &ex) {
            std::cerr << ex.displayText() << std::endl;
            return;
        }

    }


    [[nodiscard]] int CorrelationDataMode() const {
        return m_CorrelationDataMode;
    };

    void DecomposeRho() {
        // L is upper triangular
        // L = arma::chol(rho, "lower");
    }

    Eigen::MatrixXd GetRho() {
        return rho;
    }

    void SetCorr(double Corr) {
        m_SingleFactorCorrelation = Corr;
    }

    Eigen::MatrixXd GetL() {
        return L;
    }

    [[nodiscard]] double GetCorr() const {
        return m_SingleFactorCorrelation;
    };

    [[nodiscard]] int CorrelationMethod() const {
        return m_CorrelationMethod;
    }

    [[nodiscard]] int Factors() const {
        return m_Factors;
    }

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) const {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 2 | Correlation Method:" << "\t" << m_CorrelationMethod << std::endl;

        if (Level >= 3) {
            logstream.information() << ">> 3 | ........ Correlation Matrix: " << std::endl;
            for (int i = 0; i < m_Factors; i++) {
                for (int j = 0; j < m_Factors; j++) {
                }
                logstream.information() << std::endl;
            }
            logstream.information() << ">> 3 | ........ Cholesky Matrix: " << std::endl;
            for (int i = 0; i < m_Factors; i++) {
                for (int j = 0; j < m_Factors; j++) {
                }
                logstream.information() << std::endl;
            }
        }
    }

private:

    int m_CorrelationDataMode{};

    int m_CorrelationMethod{};

    size_t m_Factors{};

    double m_SingleFactorCorrelation{};
    Eigen::MatrixXd rho;

    Eigen::MatrixXd L;
};