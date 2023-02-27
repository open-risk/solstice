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

#ifndef SOLSTICE_MOBILEENTITY_H
#define SOLSTICE_MOBILEENTITY_H

#include "GenericEntity.h"
#include "entity_components/DynamicPosition.h"
#include "entity_components/Volatility.h"

class MobileEntity : public GenericEntity {

public:
    explicit MobileEntity(std::size_t size) : GenericEntity("Mobile Entity", size),
                                              m_DP(size), m_sigma(size) {

    };

    MobileEntity(Simulation &W, Model &M, Poco::LogStream &logstream) :
            GenericEntity("Mobile Entity"),
            m_DP(W, M, logstream),
            m_sigma(W, M, logstream) {
        m_Pool_Size = m_DP.GetSize();
    };

    DynamicPosition DP() {
        return m_DP;
    }

    Volatility sigma() {
        return m_sigma;
    }

    void SetDP(DynamicPosition DP) {
        m_DP = std::move(DP);
    }

    void SetX(Eigen::ArrayXd &X) { m_DP.SetX(X); }

    void Print(Poco::LogStream &logstream) override {
        m_DP.Print(logstream);
        m_sigma.Print(logstream);
    }

private:
    // Component Classes enabling Mobility
    DynamicPosition m_DP;
    Volatility m_sigma;
};


#endif //SOLSTICE_MOBILEENTITY_H
