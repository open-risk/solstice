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

#include <Poco/URI.h>

#include "models/model.h"
#include "model_component.h"

/**
 * Transition Matrix Model Component.
 */
class TransitionMatrix : public ModelComponent {
public:
    explicit TransitionMatrix(const std::string &name) : ModelComponent(name) {
        m_component_Name = "Transition Matrix";
        T.setZero();
    };

    TransitionMatrix(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Transition Matrix"){;
        std::string fragment = "TransitionMatrix";
        Initialize(fragment, W, M, logstream);
    }


    ~TransitionMatrix() override = default;

    void Deserialize(const std::string &json, Simulation &W, Model &M, Poco::LogStream &logstream) override {

        logstream.information() << ">> 1 | Transition Matrix: Parsing JSON input" << std::endl;
        logstream.information() << json << std::endl;
        logstream.information() << ">> ----------------------------" << std::endl;

        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;
        Poco::JSON::Array::Ptr JsonArray;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing transition rate data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to parse transition rate object. Check it is valid JSON"))
                                << std::endl;
            abort();
        }

        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        JsonArray = JsonObject->getArray("transition_rates");

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

        T.resize(m_Ratings, m_Ratings, m_RatingPeriods);

        Poco::JSON::Array::Ptr Tk;
        try {
            for (int k = 0; k < m_RatingPeriods; k++) {
                Tk = JsonArray->getArray(k);
                // Read Individual PD data rows
                Poco::JSON::Array::Ptr Row;
                for (int i = 0; i < m_Ratings; i++) {
                    Row = Tk->getArray(i);
                    for (int j = 0; j < m_Ratings; j++) {
                        double PD = Row->get(j);
                        T(i, j, k) = PD;
                    }
                }
            }
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to resolve transition rate values. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 3) {
            // Output multi-year probability matrices
            for (int k = 0; k < m_RatingPeriods; k++) {
                for (int i = 0; i < m_Ratings; i++) {
                    for (int j = 0; j < m_Ratings; j++)
                        logstream.information() << T(i, j, k) << "\t";
                    logstream.information() << std::endl;
                }
                logstream.information() << std::endl;
            }

        }

        if (LOG_LEVEL >= 10) {
            this->PrintModelData(LOG_LEVEL, M, logstream);
        }
        std::ofstream datatest("transition_rates_test.log");

        bool m_Debug;
        if (m_Debug) {
            for (int k = 0; k < m_RatingPeriods; k++) {
                for (int i = 0; i < m_Ratings; i++) {
                    for (int j = 0; j < m_Ratings; j++)
                        datatest << T(i, j, k) << "\t";
                    datatest << std::endl;
                }
                datatest << std::endl;
            }
        }
    }

    void Print(int Level, Model &M, Poco::LogStream &logstream){};

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 3 | Ratings: " << m_Ratings << std::endl;
        logstream.information() << ">> 3 | Periods: " << m_RatingPeriods << std::endl;
        logstream.information() << ">> 3 | Horizon: " << M.CalculationHorizon() << std::endl;

        if (Level >= 3) {
            logstream.information() << ">> 3 | ........ Transition Matrices: " << std::endl;
            logstream.precision(3);
            logstream.setf(std::ios_base::fixed, std::ios_base::floatfield);
            for (int i = 0; i < m_Ratings; i++) {
                logstream.information() << "From " << i << " to" << std::endl;
                for (int j = 0; j < m_Ratings; j++) {
                    logstream.information() << j << " : ";
                    for (int k = 0; k < m_RatingPeriods; k++) {
                        logstream.information() << T(i, j, k) << "  ";
                    }
                    logstream.information() << std::endl;
                }
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

    void Validate(Poco::LogStream &logstream, Model &M) {

        if (M.CalculationHorizon() > m_RatingPeriods && M.ModelFamilyID() != 9 && M.ModelFamilyID() != 10 &&
                M.ModelFamilyID() != 11) {
            logstream.error() << BOLD(FRED("> ERROR: Calculation Horizon exceeds Rating System Periods"))
                                << std::endl;
            abort();
        }
        if (M.GetRiskHorizon() > m_RatingPeriods) {
            logstream.error() << BOLD(FRED("> ERROR: Risk Horizon exceeds Rating System Periods")) << std::endl;
            abort();
        }
    }

    [[nodiscard]] int TransitionDataMode() const {
        return m_TransitionDataMode;
    };

    [[nodiscard]] int Ratings() const {
        return m_Ratings;
    };

    [[nodiscard]] int RatingPeriods() const {
        return m_RatingPeriods;
    };

    Eigen::Tensor<double, 3> GetT() {
        return T;
    }

private:

    int m_TransitionDataMode{};
    Eigen::Index m_RatingPeriods{}; // the total number of periods in the rating system
    Eigen::Index m_Ratings{}; // the total number of rating states in the rating system

    // Tensor Shape:  Initial Rating x Final Rating x Periods
    Eigen::Tensor<double, 3> T; // Transition Matrices
};