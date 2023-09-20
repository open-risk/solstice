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

#ifndef SOLSTICE_LossState_H
#define SOLSTICE_LossState_H

#include "entity_component.h"

/**
 * LGD / Loss State Entity Component. Provides data for the calculation and storage
 *  of loss amounts in the case of credit default
 */
class LossState : public EntityComponent {

public:
    explicit LossState(int size) : EntityComponent("Loss State") {
        m_entity_size = size;
        m_entity_Credit_Loss.setZero(size);
    }

    LossState(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Loss State") {
        std::string fragment = "LossState";
        Initialize(fragment, W, M, logstream);
    }

    void SetL(Eigen::VectorXd X) {
        m_entity_Credit_Loss = std::move(X);
    }

    [[nodiscard]] std::string GetSVG() const {
        std::string svg;
        return svg;
    }

    [[nodiscard]] int LGDClasses() const {
        return m_LGD_Classes;
    };

    void SetLGDClasses(int N) {
        m_LGD_Classes = N;
    };

private:
    // Maximum Number of LGD Classes
    // For components capturing LGD states
    int const MAX_LGD_CLASS = 5;
    int m_LGD_Classes{}; // number of LGD classes (hardwired)
    Eigen::VectorXd m_entity_Credit_Loss; // The loss amount (if any)
};

#endif //SOLSTICE_LossState_H
