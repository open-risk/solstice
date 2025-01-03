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

#ifndef SOLSTICE_IFRS9_H
#define SOLSTICE_IFRS9_H

#include "entity_component.h"

/**
 * IFRS 9 Entity Component. Allows assigning an IFRS 9 Stage attribute to a credit entity
 */
class IFRS9 : public EntityComponent {

public:
    explicit IFRS9(int size) : EntityComponent("IFRS 9") {
        m_entity_size = size;
        m_entity_Stage.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_Stage[i] = 1;
        }
    }

    IFRS9(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("IFRS 9") {
        std::string fragment = "IFRS9";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] int Stages() const {
        return m_Stages;
    };

    [[nodiscard]] int StageGap() const {
        return m_StageGap;
    };

    [[nodiscard]] int Stage(int Entity) const {
        return m_entity_Stage[Entity];
    };

    void SetStages(int N) {
        m_Stages = N;
    };

    void Deserialize(std::string json) {
        try {
            m_StageGap = MC->getValue<int>("StageGap");
        } catch (...) {
            logstream.information() << BOLD(FYEL("> WARNING: Problem reading Stage Gap. Using default value")) << std::endl;
            m_StageGap = 1;
        }
    }

private:
    // IFRS 9 System Shape
    int m_Stages{}; // number of IFRS 9 stages (hardwired)
    int m_StageGap{}; // number of rating notches for Stage 1 -> Stage 2 migration
    // IFRS 9 / CECL Related
    // Initial classification Stage
    Eigen::VectorXi m_entity_Stage;
};

#endif //SOLSTICE_IFRS9_H
