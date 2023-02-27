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

void Network::PrintEntities(int Level, Poco::LogStream &logstream) {

    if (Level >= 1) {
        logstream.information() << ">> 5 | -------- Network Data --------" << std::endl;
    }

    for (auto &pool: m_Entity_List) {
        logstream.information() << pool->GetName() << std::endl;
        logstream.information() << pool->GetSize() << std::endl;
        pool->Print(logstream);
    }

}