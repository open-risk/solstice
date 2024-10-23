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

void
Scenarios::ReadFromFile(std::string filename, Simulation &W, Model &M, Poco::LogStream &logstream) {

    const char *cstr = filename.c_str();

    if (M.ScenarioMethod() == 0) {
        logstream.information() << ">> 4 | No Scenario Data Required. Set Scenario Data Mode to zero." << std::endl;
    } else if (M.ScenarioMethod() == 1) {
        logstream.information() << ">> 4 | Simulated Scenarios. No need to read scenario data from file." << std::endl;
    }

    else if (M.ScenarioMethod() == 2 || M.ScenarioMethod() == 3 || M.ScenarioMethod() == 4) {

        if (std::filesystem::exists(filename)) {
            const char *cstr = filename.c_str();
            std::ifstream t(cstr);
            if (!t) {
                std::cerr << "ERROR: Scenario data file " << filename << " failed to open";
                std::exit(0);
            }
            std::stringstream buffer;
            buffer << t.rdbuf();
            std::string json = buffer.str();
            if (LOG_LEVEL >= 1) {
                logstream.information() << ">> 4 | Reading scenario data from: " << filename << std::endl;
            }
            this->Deserialize(json, W, M, logstream);
            logstream.information() << ">> 3 | Loaded Scenario Data from Local Files: " << filename << std::endl;
        } else {
            logstream.error() << BOLD(FRED("> ERROR: Scenario Data File does not exist")) << std::endl;
            abort();
        }
    } else {
        logstream.error() << BOLD(FRED("> ERROR: Scenario Method Not recognized.")) << std::endl;
        abort();
    }
}
