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

#include <filesystem>
#include "core/simulation.h"

void Simulation::ReadFromFile(const std::string &filename, Poco::LogStream &logstream) {

    const char *cstr = filename.c_str();

    if (std::filesystem::exists(filename)) {

        std::ifstream t(cstr);
        std::stringstream buffer;

        try {
            buffer << t.rdbuf();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading configuration data")) << std::endl;
            abort();
        };

        std::string json = buffer.str();
        input_json_string = json;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 2 | Configuration::ReadFromFile " << std::endl;
            logstream.information() << ">> 2 | Reading configuration data from: " << filename << std::endl;
        }

        this->Deserialize(json, logstream);

    } else {
        logstream.error() << BOLD(FRED("> ERROR: Simulation File does not exist")) << std::endl;
        abort();
    }
}
