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

#include "model.h"

void Model::Deserialize(const std::string &json, Poco::LogStream &logstream, Simulation &W) {

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 2 | Model::Deserialize " << std::endl;
        logstream.information() << ">> 2 | Parsing JSON input" << std::endl;
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
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem parsing model configuration data")) << std::endl;
        abort();
    }

    try {
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
    } catch (...) {
        logstream.error()
                << BOLD(FRED("> ERROR: Failed to extract model configuration objects. Check it is valid JSON"))
                << std::endl;
        abort();
    }

    Poco::JSON::Object::Ptr Model_Configuration = JsonObject->getObject("model_configuration");
    //
    // Reading L2 data from JSON serialized model configuration
    //
    try {
        m_ModelFamilyID = Model_Configuration->getValue<int>("ModelFamilyID");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading Model Family")) << std::endl;
        abort();
    }
    try {
        m_AnalyticTests = Model_Configuration->getValue<int>("Analytic_Tests");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading analytic tests")) << std::endl;
        abort();
    }
    try {
        m_ScenarioMethod = Model_Configuration->getValue<int>("ScenarioMethod");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario method")) << std::endl;
        abort();
    }
    try {
        m_MaturityMethod = Model_Configuration->getValue<int>("MaturityMethod");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading maturity method")) << std::endl;
        abort();
    }
    try {
        m_MacroScenarios = Model_Configuration->getValue<int>("MacroScenarios");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading macro scenarios")) << std::endl;
        abort();
    }
    try {
        m_PortfolioScenarios = Model_Configuration->getValue<int>("PortfolioScenarios");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading portfolio scenarios")) << std::endl;
        abort();
    }
    try {
        m_BranchScenarios = Model_Configuration->getValue<int>("BranchScenarios");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading branch scenarios")) << std::endl;
        abort();
    }

    Macro_Simulations = m_MacroScenarios * m_BranchScenarios;
    Loss_Simulations = m_MacroScenarios * m_PortfolioScenarios;
    Total_Simulations = Loss_Simulations * m_BranchScenarios;

    try {
        m_Interval = Model_Configuration->getValue<int>("Interval");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading time interval")) << std::endl;
        abort();
    }
    try {
        m_Resolution = Model_Configuration->getValue<int>("Resolution");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading resolution")) << std::endl;
        abort();
    }
    try {
        m_RiskHorizon = Model_Configuration->getValue<int>("RiskHorizon");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading risk horizon")) << std::endl;
        abort();
    }
    try {
        m_CalculationHorizon = Model_Configuration->getValue<int>("CalculationHorizon");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading calculation horizon")) << std::endl;
        abort();
    }

}



