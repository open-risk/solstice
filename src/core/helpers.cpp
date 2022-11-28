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

#ifndef SOLSTICE_HELPERS_CPP
#define SOLSTICE_HELPERS_CPP

#include <string>
#include <sstream>

#include "core/simulation.h"

[[maybe_unused]] bool invalidChar(char c) {
    return !isprint((unsigned) c);
}

std::string get_current_time() {
    /**
     * Get current date/time based on current system
     */

    time_t t = time(nullptr);
    struct tm *now = localtime(&t);
    std::stringstream current_time;

    current_time << (now->tm_year + 1900) << '-'
                 << (now->tm_mon + 1) << '-'
                 << now->tm_mday << '-'
                 << now->tm_hour << '-'
                 << now->tm_min << '-'
                 << now->tm_sec;

    std::string c_time = current_time.str();
    return c_time;
}

void Simulation::CGI_control(int level) const {
    // CGI-Mode control point
    if (m_RunLevel == level) {
        Poco::JSON::Object response;
        response.set("Run Level", level);
        response.set("Timestamp", get_current_time());
        std::stringstream payload;
        Poco::JSON::Stringifier::stringify(response, payload);
        std::cout << "Content-Type: application/json\n\n";
        std::cout << payload.str() << std::endl;
        exit(0);
    }
}

void Simulation::control_point(int level, Poco::LogStream &logstream) {
    if (m_RunLevel < level) {
        logstream.information() << "o ALL DONE! Reached RunLevel: " << m_RunLevel << std::endl;
        exit(0);
    }
}


void Simulation::
announce_level(int step, const std::string& step_string, int verbose_level, Poco::LogStream &logstream, int operating_mode) {
    if (verbose_level >= 1) {
        logstream.information() << "" << std::endl;
        if (operating_mode == 0) {
            logstream.information() << BOLD(FBLU("> ========================================")) << std::endl;
        } else {
            logstream.information() << "> ========================================" << std::endl;
        }
        logstream.information() << "> RUN LEVEL " << step << " : " << step_string << std::endl;
        if (operating_mode == 0) {
            logstream.information() << BOLD(FBLU("> ========================================")) << std::endl;
        } else {
            logstream.information() << "> ========================================" << std::endl;
        }
    }
    if (m_Debug) {
        if (m_user_operating_mode == 1) {
            CGI_control(step);
        }
        control_point(step + 1, logstream);
    }
}

void Simulation::Validate(Poco::LogStream &logstream) {
    // Note Published Configurations are Validated according to schema
    // Validation here is meant to enforce consistent choices of parameters not captured in the
    // schema for reasons of economy or other purposes
}




#endif //SOLSTICE_HELPERS_CPP