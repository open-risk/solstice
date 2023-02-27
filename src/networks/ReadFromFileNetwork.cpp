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

void Network::ReadFromFile(const std::string &filename, Simulation &W, Poco::LogStream &logstream) {

    const char *cstr = filename.c_str();

    if (std::filesystem::exists(filename)) {

        std::ifstream t(cstr);
        std::stringstream buffer;

        try {
            buffer << t.rdbuf();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading portfolio data")) << std::endl;
            abort();
        }

        std::string json = buffer.str();

        if (LOG_LEVEL >= 3) {
            logstream.information() << json << std::endl;
        }

        this->Deserialize(json, W, logstream);

    } else {
        logstream.error() << BOLD(FRED("> ERROR: Network File does not exist")) << std::endl;
    }

}


