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

#ifndef SOLSTICE_DefaultState_H
#define SOLSTICE_DefaultState_H

#include "entity_component.h"

/**
 * Default State Entity Component. Provides states that characterize credit default
 */
class DefaultState : public EntityComponent {

public:
    explicit DefaultState() : EntityComponent("Default State") {
    }

    explicit DefaultState(int size, Poco::LogStream &logstream) : EntityComponent("Default State") {
        m_entity_size = size;
        m_entity_Default_Time.setZero(size);
        logstream.information() << ">> 5 | Successfully Initialized Default State: " << m_entity_size << std::endl;
    }

    DefaultState(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Default State") {
        std::string fragment = "DefaultState";
        Initialize(fragment, W, M, logstream);
    }

    void SetDT(Eigen::VectorXi X) {
        m_entity_Default_Time = std::move(X);
    }

    [[nodiscard]] std::string GetSVG() const {
        std::string svg;
        return svg;
    }

private:
    Eigen::VectorXi m_entity_Default_Time;  // The time of default (if true)
};

#endif //SOLSTICE_DefaultState_H
