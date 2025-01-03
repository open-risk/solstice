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

#include "entity_component.h"

/*
 * Initialize and Entity Component
 */
void EntityComponent::Initialize(const std::string& fragment, Simulation &W, Model &M, Poco::LogStream &logstream) {

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 1 | Fetching Entity Component from API URL's / Local Dir Paths" << std::endl;
    }

    int operating_mode = W.getOperating_mode();
    if (operating_mode == 0 || operating_mode == 3) {
        std::string filename;
        // Read data from local files
        if (W.getPath_mode() == 1) {
            filename = W.DataDir() + "/Networks/Entity_Components/" + fragment + ".json";
        }
        this->ReadFromFile(filename, W, M, logstream);
        logstream.information() << ">> 3 | " << m_component_Name << " Component Data Read from Local Files" << std::endl;
    } else if (operating_mode == 1) {
        // Get json data from API endpoint
        std::string URL;
        this->GetFromAPI(URL, W, M, logstream);
        logstream.information() << "> " << m_component_Name << " Component Data Read from API" << std::endl;
    }
}