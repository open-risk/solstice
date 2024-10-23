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

#include "model_data.h"

void ModelComponent::GetFromAPI(const std::string &URL, Simulation &W, Model &M, Poco::LogStream &logstream) {

    std::string APItoken = W.getToken();
    std::string json;

    try {
        if (LOG_LEVEL >= 2) {
            logstream.information() << "> " << m_component_Name << " Fetching Component Data from URL: " << URL
                                << std::endl;
        }
        Poco::URI uri(W.TransitionDataURL());

        json = get_json(uri, APItoken, logstream);
    } catch (Poco::Exception &ex) {
        std::cerr << ex.displayText() << std::endl;
        return;
    }

    this->Deserialize(json, W, M, logstream);
}
