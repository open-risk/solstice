/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 Open Risk (https://www.openriskmanagement.com)
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

class FactorLoadings : public ModelComponent {
public:
    explicit FactorLoadings(const std::string &name, size_t factors) : ModelComponent(name) {
        m_component_Name = "Factor Loadings";
        // Initialize a trivial matrix by default
        m_Factors = factors;
        beta.setZero(m_Factors);
    };

    FactorLoadings(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Factor Loadings") {
        std::string fragment = "FactorLoadings";
        Initialize(fragment, W, M, logstream);
    }

    ~FactorLoadings() override = default;

    void Deserialize(const std::string &json, Model &M, Simulation &W, Poco::LogStream &logstream) {


        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;
        Poco::JSON::Array::Ptr JsonArray;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing factor loadings data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
            JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
            JsonArray = JsonObject->getArray("factor_loadings");
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to parse correlation matrix object. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        int found_factors = JsonArray->size();
        if (found_factors != m_Factors) {
            logstream.information() << BOLD(FYEL("> Warning: Matrix Size Not Consistent")) << std::endl;
        }

        try {
            m_Factors = JsonObject->getValue<int>("GetFactors");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading factor number")) << std::endl;
            abort();
        }

        try {
            m_Factors = JsonObject->getValue<int>("Factors");
            beta.resize(m_Factors);
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading factor number")) << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing loadings data from json." << std::endl;
        }

        Poco::JSON::Array::Ptr JsonArray2;

        try {
            JsonArray2 = JsonObject->getArray("loadings");
            for (int j = 0; j < m_Factors; j++) {
                double beta_value = JsonArray2->get(j);
                beta(j) = beta_value;
            }

        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to parse loadings values. Check it is valid JSON")) <<
                                std::endl;

            abort();
        }

        if (LOG_LEVEL >= 1) {

            logstream.information() << ">> 3 | Loadings Vector: " << std::endl;
            if (LOG_LEVEL >= 3) {
                for (int j = 0; j < m_Factors; j++)
                    logstream.information() << beta(j) << "\t";
                logstream.information() << std::endl;
            }
        }

    }

    [[nodiscard]] int GetFactors() const {
        return m_Factors;
    }

    Eigen::VectorXd GetBeta() {
        return beta;
    }

private:
    int m_Factors{};

    Eigen::VectorXd beta;
};