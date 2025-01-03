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

#include "network.h"
#include "entity_types/MobileEntity.h"

Network::Network(Simulation &W, Model &M, Poco::LogStream &logstream) {

    int network_shape = W.NetworkShape();
    logstream.information() << ">> 2 | Network Shape: " << network_shape << std::endl;

    switch (network_shape) {
        case 0:
            m_NetworkName = "Empty Network";
            break;
        case 5:
            m_NetworkName = "Vehicles";
            // m_Entity_List.emplace_back(std::make_shared<MobileEntity>(1));
            m_Entity_List.emplace_back(std::make_shared<MobileEntity>(W, M, logstream));
            break;
        default:
            logstream.error() << "ERROR IN NETWORK SHAPE!" << std::endl;
            logstream.information() << ">> CREATED ENTITIES" << std::endl;
            logstream.information() << ">> NETWORK SHAPE: " << network_shape << std::endl;
            logstream.information() << "====================================" << std::endl;
            logstream.information() << this->GetNetworkName() << std::endl;
            logstream.information() << this->GetEntityCount() << std::endl;
            break;
    };

    PrintEntities(2, logstream);

}