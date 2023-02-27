/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2023 Open Risk (https://www.openriskmanagement.com)+
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

class TransitionThresholds : public ModelComponent {
public:

    explicit TransitionThresholds(const std::string &name) : ModelComponent(name) {
        m_component_Name = "Transition Thresholds";
        A.setZero();
    };

    TransitionThresholds(const std::string &name, Eigen::Index ratings, Eigen::Index periods) : ModelComponent(name) {
        m_Ratings = ratings;
        m_RatingPeriods = periods;
        A.resize(ratings, ratings, periods);
    };

    TransitionThresholds(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Transition Thresholds") {
        std::string fragment = "TransitionThresholds";
        Initialize(fragment, W, M, logstream);
    }

    ~TransitionThresholds() override = default;

    void Deserialize(const std::string &json, Simulation &W, Model &M, Poco::LogStream &logstream) override {

        logstream.information() << ">> 1 | Transition Thresholds: Parsing JSON input" << std::endl;
        logstream.information() << json << std::endl;
        logstream.information() << ">> ----------------------------" << std::endl;

        Poco::JSON::Parser jsonParser;

        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;
        Poco::JSON::Array::Ptr JsonArray;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing transition thresholds data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to parse transition thresholds object. Check it is valid JSON"))
                                << std::endl;
            abort();
        }

        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        JsonArray = JsonObject->getArray("transition_thresholds");

        try {
            m_RatingPeriods = JsonObject->getValue<int>("RatingPeriods");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading periods")) << std::endl;
            abort();
        }
        try {
            m_Ratings = JsonObject->getValue<int>("Ratings");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading ratings")) << std::endl;
            abort();
        }

        A.resize(m_Ratings, m_Ratings, m_RatingPeriods);

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing transition threshold data." << std::endl;
        }

        Poco::JSON::Array::Ptr Th;

        try {
            for (int k = 0; k < m_RatingPeriods; k++) {
                Th = JsonArray->getArray(k);
                Poco::JSON::Array::Ptr Row;
                for (int i = 0; i < m_Ratings; i++) {
                    Row = Th->getArray(i);
                    for (int j = 0; j < m_Ratings; j++) {
                        double TH = Row->get(j);
                        A(i, j, k) = TH;
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
                for (int i = 0; i < m_Ratings; i++) {
                    for (int j = 0; j < m_Ratings; j++)
                        logstream.information() << A(i, j, k) << "\t";
                    logstream.information() << std::endl;
                }
                logstream.information() << std::endl;
            }
        }

        if (LOG_LEVEL >= 2) {
            this->PrintModelData(LOG_LEVEL, M, logstream);
        }
        std::ofstream datatest("transition_thresholds_test.log");

        if (m_Debug) {
            // Output multi-year probability matrices
            for (int k = 0; k < m_RatingPeriods; k++) {
                for (int i = 0; i < m_Ratings; i++) {
                    for (int j = 0; j < m_Ratings; j++)
                        datatest << A(i, j, k) << "\t";
                    datatest << std::endl;
                }
                datatest << std::endl;
            }
        }
    }

    void ThresholdsFromJSON(Model &M, Simulation &W, Poco::LogStream &logstream) {

        std::string filename3 = W.ThresholdDataURL();
        if (LOG_LEVEL >= 10) {
            logstream.information() << ">> 3 | Reading threshold data from: " << filename3 << std::endl;
        }

        const char *cstr3 = filename3.c_str();
        std::ifstream t3(cstr3);
        std::stringstream buffer3;
        try {
            buffer3 << t3.rdbuf();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading transition threshold data")) << std::endl;
            abort();
        };
        std::string json3 = buffer3.str();

        Poco::JSON::Parser jsonParser2;
        Poco::Dynamic::Var ParsedJson2;

        try {
            ParsedJson2 = jsonParser2.parse(json3);
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem parsing transition threshold data")) << std::endl;
            abort();
        };

        Poco::Dynamic::Var ParsedJsonResult2 = jsonParser2.result();

        Poco::JSON::Array::Ptr Th;
        Poco::JSON::Array::Ptr JsonArray2;

        try {
            JsonArray2 = ParsedJsonResult2.extract<Poco::JSON::Array::Ptr>();
            for (int k = 0; k < m_RatingPeriods; k++) {
                Th = JsonArray2->getArray(k);
                // Read Individual PD data rows
                Poco::JSON::Array::Ptr Row;
                for (int i = 0; i < m_Ratings; i++) {
                    Row = Th->getArray(i);
                    for (int j = 0; j < m_Ratings; j++) {
                        double TH = Row->get(j);
                        A(i, j, k) = TH;
                    }
                }
            }
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Unexpected structure for thresholds matrix")) << std::endl;
            abort();
        }


    }

    Eigen::Tensor<double, 3> GetA() {
        return A;
    }

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 3 | Ratings: " << m_Ratings << std::endl;
        logstream.information() << ">> 3 | Periods: " << m_RatingPeriods << std::endl;
        logstream.information() << ">> 3 | Horizon: " << M.CalculationHorizon() << std::endl;

        if (Level >= 3) {
            logstream.information() << ">> 3 | ........ Transition Thresholds: " << std::endl;
            for (int i = 0; i < m_Ratings; i++) {
                logstream.information() << "From " << i << " to" << std::endl;
                for (int j = 0; j < m_Ratings; j++) {
                    logstream.information() << j << " : ";
                    for (int k = 0; k < m_RatingPeriods; k++) {
                        logstream.information() << A(i, j, k) << "  ";
                    }
                    logstream.information() << std::endl;
                }
            }
        }
    }

    void ThresholdsToJSONFile(Model &M, Simulation &W){};

    void CalibrateThresholds(int ri, Simulation &W){};

private:
    Eigen::Index m_RatingPeriods{};
    Eigen::Index m_Ratings{};
    Eigen::Tensor<double, 3> A; // Thresholds
};


