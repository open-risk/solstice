/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
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

class CreditCurve : public ModelComponent {
public:
    explicit CreditCurve(const std::string &name) : ModelComponent(name) {
        m_component_Name = "Credit Curve";
        CC.setZero();
    };

    CreditCurve(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Credit Curve"){
        std::string fragment = "CreditCurve";
        Initialize(fragment, W, M, logstream);
    }

    CreditCurve(const std::string &name, Eigen::Index ratings, Eigen::Index periods) : ModelComponent(name) {
        m_Ratings = ratings;
        m_RatingPeriods = periods;
        CC.resize(ratings, periods);
    };

    ~CreditCurve() override = default;

    void Deserialize(){};

    void PDFromJSONFile(std::string PDfilename){};

    void SetPD(Model &M, Simulation &W){};

    [[nodiscard]] int Ratings() const {
        return m_Ratings;
    };

    [[nodiscard]] int RatingPeriods() const {
        return m_RatingPeriods;
    };

    Eigen::MatrixXd GetCC() {
        return CC;
    }

    void TMFromAPI(std::string APItoken, Model &M, Simulation &W, Poco::LogStream &logstream) {
        try {

            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 3 | Fetching rating transition data via API " << std::endl;
            }

            Poco::URI uri(W.TransitionDataURL());

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
            std::stringstream transition_matrix_data;
            Poco::StreamCopier::copyStream(is, transition_matrix_data);
            std::string json = transition_matrix_data.str();

            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 3 | Transition Data Server Response " << std::endl;
                logstream.information() << ">> 3 | " << res.getStatus() << " " << res.getReason() << std::endl;
                if (LOG_LEVEL >= 3) {
                    logstream.information() << json << std::endl;
                }
            }

            this->TMFromJSON(json, M, W, logstream);


        } catch (Poco::Exception &ex) {
            std::cerr << ex.displayText() << std::endl;
            return;
        }

    }

    void TMFromJSON(std::string json, Model &M, Simulation &W, Poco::LogStream &logstream) {

        Poco::JSON::Parser jsonParser;

        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;
        Poco::JSON::Array::Ptr JsonArray;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing credit curve data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
            JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
            JsonArray = JsonObject->getArray("transition_rates");
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to parse credit curve object. Check it is valid JSON"))
                                << std::endl;
            abort();
        }

        Poco::JSON::Array::Ptr Tk;
        try {
            for (int k = 0; k < m_RatingPeriods; k++) {
                Tk = JsonArray->getArray(k);
                for (int j = 0; j < m_Ratings; j++) {
                    double PD = Tk->get(j);
                    CC(j, k) = PD;
                }
            }
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to resolve transition rate values. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing transition threshold data." << std::endl;
        }

        Poco::JSON::Array::Ptr JsonArray2;
        Poco::JSON::Array::Ptr Th;

        try {
            JsonArray2 = JsonObject->getArray("transition_thresholds");
            for (int k = 0; k < m_RatingPeriods; k++) {
                Th = JsonArray2->getArray(k);
                Poco::JSON::Array::Ptr Row;
                for (int i = 0; i < m_Ratings; i++) {
                    Row = Th->getArray(i);
                    for (int j = 0; j < m_Ratings; j++) {
                        double TH = Row->get(j);
//                        A(i, j, k) = TH;
                    }
                }
            }
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to parse transition threshold values. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 3) {
            for (int k = 0; k < m_RatingPeriods; k++) {
                for (int j = 0; j < m_Ratings; j++)
                    logstream.information() << CC(j, k) << "\t";
                logstream.information() << std::endl;
            }

            for (int k = 0; k < m_RatingPeriods; k++) {
                for (int j = 0; j < m_Ratings; j++)
//                        logstream.information() << A(i, j, k) << "\t";
                    logstream.information() << std::endl;

            }
        }
    }

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 3 | Ratings: " << m_Ratings << std::endl;
        logstream.information() << ">> 3 | Periods: " << m_RatingPeriods << std::endl;
        logstream.information() << ">> 3 | Horizon: " << M.CalculationHorizon() << std::endl;

        if (Level >= 3) {
            logstream.information() << ">> 3 | ........ Credit Curve: " << std::endl;
            logstream.precision(3);
            logstream.setf(std::ios_base::fixed, std::ios_base::floatfield);
            for (int j = 0; j < m_Ratings; j++) {
                logstream.information() << j << " : ";
                for (int k = 0; k < m_RatingPeriods; k++) {
                    logstream.information() << CC(j, k) << "  ";
                }
                logstream.information() << std::endl;
            }

            logstream.information() << ">> 3 | ........ Transition Thresholds: " << std::endl;
            for (int i = 0; i < m_Ratings; i++) {
                logstream.information() << "From " << i << " to" << std::endl;
                for (int j = 0; j < m_Ratings; j++) {
                    logstream.information() << j << " : ";
                    for (int k = 0; k < m_RatingPeriods; k++) {
//                        logstream.information() << A(i, j, k) << "  ";
                    }
                    logstream.information() << std::endl;
                }
            }
        }
    }

private:

    Eigen::Index m_RatingPeriods{}; // the total number of periods in the rating system
    Eigen::Index m_Ratings{}; // the total number of rating states in the rating system
    Eigen::MatrixXd CC; // Credit Curve
};