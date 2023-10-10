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

#ifndef ASSETRETURN_H
#define ASSETRETURN_H

#include <utility>

#include "entity_component.h"

/**
 * Asset Return Entity Component.
 * Can be used for Threshold Based Default Risk Calculations, Collateral Valuation etc.
 *
 */
class AssetReturn : public EntityComponent {

public:
    explicit AssetReturn() : EntityComponent("Asset Return") {
    }
    explicit AssetReturn(int size) : EntityComponent("Asset Return") {
        m_entity_size = size;
        r.setZero(size);
    }

    AssetReturn(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Asset Return") {
        std::string fragment = "AssetReturn";
        Initialize(fragment, W, M, logstream);
    }

    [[nodiscard]] Eigen::VectorXd GetR() const {
        return r;
    }

    void SetR(Eigen::VectorXd X) {
        r = std::move(X);
    }

    void Print(Poco::LogStream &logstream) {
        for (int i = 0; i < m_entity_size; i++)
            logstream.information() << i << " " << r[i] << std::endl;
    }

private:
    Eigen::VectorXd r;
};

#endif //ASSETRETURN_H