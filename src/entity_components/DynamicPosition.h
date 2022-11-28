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

#ifndef SOLSTICE_DYNAMICPOSITION_H
#define SOLSTICE_DYNAMICPOSITION_H

#include "entity_component.h"


class DynamicPosition : public EntityComponent {

public:
    explicit DynamicPosition(std::size_t size) : EntityComponent("Dynamic Position") {
        m_entity_size = size;
        m_entity_X.setZero(size);
        m_entity_Y.setZero(size);
        m_entity_VX.setZero(size);
        m_entity_VY.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_X[i] = 0.0;
            m_entity_Y[i] = 0.0;
            m_entity_VX[i] = 1.0;
            m_entity_VY[i] = 1.0;
        }
    }

    DynamicPosition(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Dynamic Position") {
        std::string fragment = "DynamicPosition";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] auto GetX() const {
        return m_entity_X;
    }

    [[nodiscard]] double GetX(Eigen::Index i) const {
        return m_entity_X[i];
    }

    void SetX(Eigen::ArrayXd &X) {
        m_entity_X = X;
    }
    void SetX(double x, Eigen::Index i) {
        m_entity_X[i] = x;
    }

    [[nodiscard]] Eigen::ArrayXd GetY() const {
        return m_entity_Y;
    }
    void SetY(Eigen::ArrayXd X) {
        m_entity_Y = std::move(X);
    }

    void SetVX(Eigen::ArrayXd X) {
        m_entity_VX = std::move(X);
    }

    void SetVY(Eigen::ArrayXd X) {
        m_entity_VY = std::move(X);
    }

    [[nodiscard]] Eigen::ArrayXd GetVX() const {
        return m_entity_VX;
    }

    Eigen::ArrayXd GetVY() const {
        return m_entity_VY;
    }

    static std::string Serialize() {
        // create json object from
        // serialize json object
        return "SERIALIZED";
    }

    void Print(Poco::LogStream &logstream) {
        logstream.information() << m_component_Name << std::endl;
        logstream.information() << "----------------------------" << std::endl;
        for (int k = 0; k < m_entity_size; k++)
            logstream.information() << k << " " << m_entity_X[k] << std::endl;
    }

private:
    Eigen::ArrayXd m_entity_X;
    Eigen::ArrayXd m_entity_Y;
    Eigen::ArrayXd m_entity_VX;
    Eigen::ArrayXd m_entity_VY;
};

#endif //SOLSTICE_DYNAMICPOSITION_H
