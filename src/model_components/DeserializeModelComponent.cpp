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


void ModelComponent::Deserialize(const std::string& json, Simulation &W, Model &M, Poco::LogStream &logstream) {

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 3 | === Generic Model Component JSON String Parser === " << std::endl;
    }
    if (LOG_LEVEL >= 3) {
        logstream.information() << json << std::endl;
    }

    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;
    Poco::JSON::Object::Ptr Model_Data;

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        Model_Data = JsonObject->getObject("model_data");
    }
    catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to parse model component data. Check it is valid JSON"))
                            << std::endl;
        abort();
    }

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 3 | -------- Model Component Data --------" << std::endl;
        logstream.information() << ">> 3 | Horizon: " << M.CalculationHorizon() << std::endl;
    }
}


