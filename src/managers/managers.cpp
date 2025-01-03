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

#include "managers.h"

Managers::Managers(Simulation &W, Model &M, Poco::LogStream &logstream)
        : GenericEntity("Manager", 1) {

    if (W.LiabilityDataMode() == 0) {
        logstream.information() << ">> 6 | No Liability Data Required" << std::endl;
    } else {
        logstream.information() << ">> 6 | Fetching Liability Data from API" << std::endl;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 1 | Fetching Manager Data from API URL's / Local Dir Paths" << std::endl;
        }

        Initialize(W, M, logstream);

    }

}

