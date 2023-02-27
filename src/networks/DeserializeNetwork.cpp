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
#include "networks/network.h"

void Network::Deserialize(const std::string &json, Poco::LogStream &logstream) {


    Poco::JSON::Parser jsonParser;

    if (LOG_LEVEL >= 3) {
        logstream.information() << json << std::endl;
    }

    try {
        jsonParser.parse(json);
        Poco::Dynamic::Var ParsedJsonResult = jsonParser.result();
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem parsing portfolio data")) << std::endl;
        abort();
    };


    Poco::JSON::Array::Ptr arr;
    Poco::JSON::Object::Ptr object;

    try {
        m_NetworkDataMode = object->getValue<int>("Portfolio_data_mode");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading portfolio data mode")) << std::endl;
        abort();
    }


}


