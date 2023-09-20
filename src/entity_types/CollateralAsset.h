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

#ifndef SOLSTICE_CollateralAsset_H
#define SOLSTICE_CollateralAsset_H

#include "GenericEntity.h"
#include "entity_components/AssetReturn.h"

/**
 *  An entity that has an asset return (used as collateral)
 */
class CollateralAsset : public GenericEntity {

public:
    explicit CollateralAsset(std::size_t size) : GenericEntity("Collateral Asset", size),
                                                 m_AR(size) {
    };

    explicit CollateralAsset(Simulation &W, Model &M, Poco::LogStream &logstream) :
            GenericEntity("Collateral Asset"),
            m_AR(W, M, logstream) {
    };

private:
    AssetReturn m_AR;
};


#endif //SOLSTICE_CollateralAsset_H
