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

#include "core/simulation.h"

void Simulation::Deserialize(const std::string &json, Poco::LogStream &logstream) {

    logstream.information() << ">> 3 | Configuration: Parsing JSON input" << std::endl;
    logstream.trace() << json << std::endl;
    logstream.trace() << ">> ----------------------------" << std::endl;

    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to parse configuration. Check it is valid JSON"))
                          << std::endl;
        abort();
    }

    try {
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to extract configuration objects. Check it is valid JSON"))
                          << std::endl;
        abort();
    }

    //
    // Reading L1 data from JSON serialized configuration
    //
    try {
        m_NetworkShape = JsonObject->getValue<int>("network_shape");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading network shape")) << std::endl;
        abort();
    }
    try {
        m_WorkflowName = JsonObject->getValue<std::string>("workflow_name");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading workflow name")) << std::endl;
        abort();
    }
    try {
        m_WorkflowID = JsonObject->getValue<std::string>("workflow_id");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading workflow ID")) << std::endl;
        abort();
    }
    try {
        m_Description = JsonObject->getValue<std::string>("workflow_description");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading workflow description")) << std::endl;
        abort();
    }
    try {
        m_RunLevel = JsonObject->getValue<int>("run_level");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading run level")) << std::endl;
        abort();
    }

    //
    // The input settings L2 section
    //

    Poco::JSON::Object::Ptr Input_Set = JsonObject->getObject("input_settings");

    //
    // Data URL's
    //

    try {
        m_BaseAPIURL = Input_Set->getValue<std::string>("BaseAPIURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading Base API URL")) << std::endl;
        abort();
    }
    try {
        m_DataDir = Input_Set->getValue<std::string>("DataDir");
    } catch (...) {
        logstream.information() << BOLD(FYEL("> Warning: Problem reading Data Dir. Using default")) << std::endl;
        m_DataDir = "./";
    }

    // OPTIONAL DATA SOURCES (SCENARIOS / MANAGERS)

    try {
        m_LiabilityDataMode = Input_Set->getValue<int>("Manager_data_mode");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading manager data mode")) << std::endl;
        abort();
    }
    try {
        m_LiabilitiesURL = Input_Set->getValue<std::string>("ManagerDataURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading manager data URL")) << std::endl;
        abort();
    }
    try {
        m_ScenarioDataURL = Input_Set->getValue<std::string>("ScenarioDataURL");
    } catch (...) {

        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario data URL")) << std::endl;
        abort();
    }
    try {
        m_ScenarioDataMode = Input_Set->getValue<int>("Scenario_data_mode");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario data mode")) << std::endl;
        abort();
    }
    try {
        m_PortfolioURL = Input_Set->getValue<std::string>("PortfolioURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading portfolio URL")) << std::endl;
        abort();
    }
    try {
        m_CorrelationURL = Input_Set->getValue<std::string>("CorrelationURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading correlation matrix URL")) << std::endl;
    }
    try {
        m_LoadingsURL = Input_Set->getValue<std::string>("LoadingsURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading loadings URL")) << std::endl;
    }
    try {
        m_AdjacencyURL = Input_Set->getValue<std::string>("AdjacencyURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading adjacency matrix URL")) << std::endl;
    }
    try {
        m_ThresholdDataURL = Input_Set->getValue<std::string>("ThresholdDataURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading threshold data URL")) << std::endl;
        abort();
    }
    try {
        m_TransitionDataURL = Input_Set->getValue<std::string>("TransitionDataURL");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading transition data URL")) << std::endl;
        abort();
    }


    //
    // The output settings L2 section
    //
    Poco::JSON::Object::Ptr Output_Set = JsonObject->getObject("output_settings");
    try {
        m_ResultsURL = Output_Set->getValue<std::string>("Results_url");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading results url")) << std::endl;
        abort();
    }
    try {
        m_ReportingMode = Output_Set->getValue<int>("Reporting_mode");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading reporting mode")) << std::endl;
        abort();
    }
    try {
        m_Verbose_Level = Output_Set->getValue<int>("Verbose_Level");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading verbosity level")) << std::endl;
        abort();
    }
    try {
        Poco::JSON::Array::Ptr arr;
        arr = Output_Set->getArray("Results_list");
        int result_count = arr->size();
        m_ResultsList.setZero(result_count);
        for (int i = 0; i < result_count; i++) {
            m_ResultsList[i] = arr->get(i);
        }
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading results list")) << std::endl;
        abort();
    }
}

