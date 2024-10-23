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

#ifndef SOLSTICE_CREDITENTITY_H
#define SOLSTICE_CREDITENTITY_H

#include "GenericEntity.h"
#include "entity_components/DefaultState.h"

/**
 *  An entity that has a credit state
 */
class CreditEntity : public GenericEntity {

public:
    CreditEntity(std::size_t size, Poco::LogStream &logstream) : GenericEntity("Credit Entity", size),
                                     m_CS(size, logstream) {

    };
    explicit CreditEntity(Simulation &W, Model &M, Poco::LogStream &logstream) :
    GenericEntity("Credit Entity"),
    m_CS(W, M, logstream) {
    };


private:
    // Component Classes enabling Credit characterisation
    DefaultState m_CS;
};

#endif //SOLSTICE_CREDITENTITY_H
