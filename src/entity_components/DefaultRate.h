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

#ifndef SOLSTICE_DefaultRate_H
#define SOLSTICE_DefaultRate_H

#include "entity_component.h"

/**
 * Default Rate Entity Component. Provides states that characterizes credit default rates for entity pools
 */
class DefaultRate : public EntityComponent {

public:
    explicit DefaultRate(int size) : EntityComponent("Default State") {
        m_entity_size = size;
    }

    DefaultRate(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Default Rate") {
        std::string fragment = "DefaultRate";
        Initialize(fragment, W, M, logstream);
    }

    void SetDR(Eigen::VectorXd X) {
        m_entity_Default_Rate = std::move(X);
    }

private:
    Eigen::VectorXd m_entity_Default_Rate;  // The default rate of a homogeneous entity pool
};

#endif //SOLSTICE_DefaultRate_H
