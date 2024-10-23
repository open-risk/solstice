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

#include "entity_component.h"

void EntityComponent::ReadFromFile(const std::string &filename, Simulation &W, Model &M, Poco::LogStream &logstream) {

    const char *cstr = filename.c_str();

    if (LOG_LEVEL >= 1) {
        logstream.information() << "> " << m_component_Name << " Reading Component Data from Local File: "
                            << filename << std::endl;
    }
    // open file
    if (std::filesystem::exists(filename)) {

        std::ifstream t(cstr);
        std::stringstream buffer;

        try {
            buffer << t.rdbuf();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading entity component data")) << std::endl;
            abort();
        };

        std::string json = buffer.str();

        // parse file contents
        this->Deserialize(json, W, M, logstream);

    } else {
        logstream.error() << BOLD(FRED("> ERROR: Entity Component File does not exist")) << std::endl;
        abort();
    }
};