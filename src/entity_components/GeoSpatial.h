/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
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

#ifndef SOLSTICE_GeoSpatial_H
#define SOLSTICE_GeoSpatial_H

#include "entity_component.h"

/**
 * Geospatial Entity Component. Allows locating entities on a 2D map
 */
class GeoSpatial : public EntityComponent {

public:
    explicit GeoSpatial(int size) : EntityComponent("GeoSpatial") {
        m_entity_size = size;
        m_entity_X.setZero(size);
        m_entity_Y.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_X[i] = 1.0;
            m_entity_Y[i] = 2.0;
        }
    }

    GeoSpatial(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("GeoSpatial") {
        std::string fragment = "Geospatial";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] Eigen::ArrayXd GetX() const {
        return m_entity_X;
    }

    [[nodiscard]] Eigen::ArrayXd GetY() const {
        return m_entity_Y;
    }

    [[nodiscard]] int X(int Entity) const {
        return m_entity_X[Entity];
    };

    [[nodiscard]] int Y(int Entity) const {
        return m_entity_Y[Entity];
    };

private:
    // Virtual Grid information
    Eigen::ArrayXd m_entity_X;
    Eigen::ArrayXd m_entity_Y;
};

#endif //SOLSTICE_GeoSpatial_H
