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

#include "managers.h"

void Managers::ReadFromFile(std::string filename, Simulation &W, Model &M, Poco::LogStream &logstream) {

    const char *cstr = filename.c_str();

    if (LOG_LEVEL >= 1) {
        logstream.information() << ">> 6 | Reading manager structure from: " << filename << std::endl;
    }
    // open file
    if (std::filesystem::exists(filename)) {

        std::ifstream t(cstr);
        std::stringstream buffer;
        try {
            buffer << t.rdbuf();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading manager data")) << std::endl;
            abort();
        };

        std::string json = buffer.str();

        if (LOG_LEVEL >= 3) {
            logstream.information() << json << std::endl;
        }

        this->Deserialize(json, W, logstream);

    } else {
        logstream.error() << BOLD(FRED("> ERROR: Manager File does not exist")) << std::endl;
        abort();
    }

}
