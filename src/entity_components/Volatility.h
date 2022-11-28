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

#ifndef SOLSTICE_Volatility_H
#define SOLSTICE_Volatility_H

#include "entity_component.h"

class Volatility : public EntityComponent {

public:
    explicit Volatility(int size) : EntityComponent("Volatility") {
        m_entity_size = size;
        m_sigma.setZero(size);
        for (int i = 0; i < size; i++) {
            m_sigma[i] = 1.0;
        }
    }

    Volatility(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Volatility") {
        std::string fragment = "Volatility";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] Eigen::ArrayXd GetSigma() const {
        return m_sigma;
    }
    [[nodiscard]] double Sigma(int Entity) const {
        return m_sigma[Entity];
    };

    void Print(Poco::LogStream &logstream) {
        logstream.information() << m_component_Name << std::endl;
        logstream.information() << "----------------------------" << std::endl;
        for (int k = 0; k < m_entity_size; k++)
            logstream.information() << k << " " << m_sigma[k] << std::endl;
    }

private:
    // volatility metric
    Eigen::ArrayXd m_sigma;
};


#endif //SOLSTICE_Volatility_H
