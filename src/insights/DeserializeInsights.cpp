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

#include "insights.h"

void Insights::Deserialize(const std::string json, Poco::LogStream &logstream) {

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 1 | Configuration: Parsing JSON input" << std::endl;
    }
    if (LOG_LEVEL >= 3) {
        logstream.trace() << json << std::endl;
        logstream.information() << ">> ----------------------------" << std::endl;
    }

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
    // The output set section
    //

    Poco::JSON::Object::Ptr Output_Set = JsonObject->getObject("output_settings");

    try {
        m_ScenarioOutput = Output_Set->getValue<int>("Scenario_output");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading scenario output")) << std::endl;
        abort();
    }

}

