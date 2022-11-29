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

#ifndef SOLSTICE_DefaultThreshold_H
#define SOLSTICE_DefaultThreshold_H

#include <utility>

#include "entity_component.h"

class DefaultThreshold : public EntityComponent {

public:
    explicit DefaultThreshold() : EntityComponent("Default Threshold") {
    }

    explicit DefaultThreshold(int size) : EntityComponent("Default Threshold") {
        m_entity_size = size;
        a.setZero(size);
    }

    DefaultThreshold(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Default Threshold") {
        std::string fragment = "DefaultThreshold";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] Eigen::VectorXd GetA() const {
        return a;
    }
    void SetA(Eigen::VectorXd &X) {
        a = std::move(X);
    }
    void Print(Poco::LogStream &logstream) {
        for (int k = 0; k < m_entity_size; k++)
            logstream.information() << k << " " << a[k] << std::endl;
    }

private:
    Eigen::VectorXd a;
};

#endif //SOLSTICE_DefaultThreshold_H
