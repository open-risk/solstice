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

#ifndef SOLSTICE_CORPORATECREDITENTITY_H
#define SOLSTICE_CORPORATECREDITENTITY_H

#include "GenericEntity.h"
#include "entity_components/DefaultState.h"
#include "entity_components/BusinessModel.h"
#include "entity_components/BaselParameters.h"
#include "entity_components/AssetReturn.h"
#include "entity_components/DefaultThreshold.h"
#include "entity_components/CreditRating.h"

/**
 *  Credit Corporate Entity. A corporate entity that has assigned dynamic and static
 *  credit attributes.
 */
class CorporateCreditEntity : public GenericEntity {

public:
    explicit CorporateCreditEntity(std::size_t size, Poco::LogStream &logstream) :
            GenericEntity("Corporate Credit Entity", size),
            m_BP(size),
            m_BM(size),
            m_DS(size, logstream),
            m_AR(size),
            m_DT(size),
            m_CR(size)
            {
                m_Pool_Size = size;
            };

    explicit CorporateCreditEntity(Simulation &W, Model &M, Poco::LogStream &logstream) :
            GenericEntity("Corporate Credit Entity"),
            m_BP(W, M, logstream),
            m_BM(W, M, logstream),
            m_CR(W, M, logstream),
            m_DS(m_BP.GetSize(), logstream),
            m_AR(m_BP.GetSize()),
            m_DT(m_BP.GetSize())
            {
                AddComponent(m_BP.GetName());
                AddComponent(m_BM.GetName());
                AddComponent(m_CR.GetName());
                AddComponent(m_DS.GetName());
                AddComponent(m_AR.GetName());
                AddComponent(m_DT.GetName());
                SetSize(m_BP.GetSize());
            };

    BaselParameters BP() {
        return m_BP;
    }

    BusinessModel BM() {
        return m_BM;
    }

    DefaultState DS() {
        return m_DS;
    }

    AssetReturn AR() {
        return m_AR;
    }

    CreditRating CR() {
        return m_CR;
    }

    DefaultThreshold DT() {
        return m_DT;
    }

    // Object update
    void SetDS(DefaultState DS) {
        m_DS = std::move(DS);
    }

private:
    // Component Classes enabling standalone credit characterisation of a Corporate Entity
    // Use: Regulatory Capital calculations, Concentration Analysis

    BaselParameters m_BP;
    BusinessModel m_BM;
    CreditRating m_CR;
    DefaultState m_DS;
    AssetReturn m_AR;
    DefaultThreshold m_DT;
};

#endif //SOLSTICE_CORPORATECREDITENTITY_H
