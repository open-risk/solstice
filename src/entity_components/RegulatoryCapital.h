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

#ifndef SOLSTICE_REGULATORYCAPITAL_H
#define SOLSTICE_REGULATORYCAPITAL_H

#include "entity_component.h"

/**
 * Regulatory Capital Entity Component. Associates with a credit entity regulatory capital
 * parameters
 */
class RegulatoryCapital : public EntityComponent {

public:
    RegulatoryCapital(int size) : EntityComponent("Regulatory Capital") {
        m_entity_size = size;
        m_entity_RW.setZero(size);
        m_entity_RWA.setZero(size);
    }

    RegulatoryCapital(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Regulatory Capital") {
        std::string fragment = "RegulatoryCapital";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] Eigen::VectorXd GetRW() const {
        return m_entity_RW;
    }

    [[nodiscard]] Eigen::VectorXd GetRWA() const {
        return m_entity_RW;
    }

    void Serialize() {
    }

private:
    Eigen::VectorXd m_entity_RW;   // Risk Weight
    Eigen::VectorXd m_entity_RWA;  // Risk Weight Assets
};


#endif //SOLSTICE_REGULATORYCAPITAL_H
