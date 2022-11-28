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

#ifndef SOLSTICE_POOLEDCREDITENTITY_H
#define SOLSTICE_POOLEDCREDITENTITY_H

#include "GenericEntity.h"
#include "entity_components/DefaultRate.h"

class PooledCreditEntity : public GenericEntity {

public:
    explicit PooledCreditEntity(std::size_t size) : GenericEntity("Pooled Credit Entity", size),
                                           m_DR(size) {

    };

    explicit PooledCreditEntity(Simulation &W, Model &M, Poco::LogStream &logstream) :
            GenericEntity("Pooled Credit Entity"),
            m_DR(W, M, logstream) {
    };

private:
    DefaultRate m_DR;
};

#endif //SOLSTICE_POOLEDCREDITENTITY_H
