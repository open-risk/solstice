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

#include "scenarios.h"

void Scenarios::Deserialize(const std::string& json, Simulation &W, Model &M, Poco::LogStream &logstream) {

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 4 | Scenario Data JSON String Parser" << std::endl;
    }
    if (LOG_LEVEL >= 3) {
        logstream.information() << json << std::endl;
        logstream.information() << ">> ----------------------------" << std::endl;
    }

    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem parsing scenario data")) << std::endl;
        abort();
    }

    Poco::JSON::Array::Ptr Factor_Data;
    Poco::JSON::Object::Ptr Factor;
    Poco::JSON::Array::Ptr Factor_Values;
    Poco::JSON::Array::Ptr Factor_Probabilities;

    try {
        m_ScenarioPeriods = JsonObject->getValue<int>("timepoint_no");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario periods")) << std::endl;
        abort();
    };

    try {

        m_Factors = 1;

        m_FactorScenarios = JsonObject->getValue<int>("scenario_no");

        m_FactorData.setZero(m_FactorScenarios, m_ScenarioPeriods + 1);
        m_FactorProbabilities.setZero(m_FactorScenarios);

        Factor_Data = JsonObject->getArray("factor_data");
        Factor_Probabilities = JsonObject->getArray("scenario_probabilities");

        if (m_ScenarioPeriods < M.CalculationHorizon()) {
            logstream.information() << BOLD(FRED(
                    "> ERROR: Temporal grid misconfiguration, scenario data periods less than configured calculation horizon"))
                                << std::endl;
            abort();
        }

        int found_scenarios = Factor_Data->size();
        if (found_scenarios != m_FactorScenarios) {
            logstream.error() << BOLD(FRED("> ERROR: Did not find expect number of scenarios")) << std::endl;
            abort();
        }

        for (int f = 0; f < m_FactorScenarios; f++) {
            Factor = Factor_Data->getObject(f);
            Factor_Values = Factor->getArray("Factor");
            m_FactorProbabilities(f) = Factor_Probabilities->get(f);
            for (int k = 0; k < m_ScenarioPeriods; k++) {
                double value = Factor_Values->get(k);
                m_FactorData(f, k) = value;
            }
        }
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario data")) << std::endl;
        abort();
    }

    if (LOG_LEVEL >= 1) {
        logstream.information() << ">> 4 | -------- Scenario Data --------" << std::endl;
        logstream.information() << ">> 4 | Periods: " << m_ScenarioPeriods << std::endl;
        logstream.information() << ">> 4 | Factor Scenarios: " << m_FactorScenarios << std::endl;
    }

}


