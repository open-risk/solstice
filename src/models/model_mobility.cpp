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

#include "model.h"
#include "entity_types/MobileEntity.h"
#include "insights/RandomVar1D.h"

void
Model::model_mobility(Network &V,
                      Scenarios &S,
                      Model_Data &MD,
                      Managers &MA,
                      Insights &MR,
                      Poco::LogStream &logstream) {

    logstream.information() << ">> 8 | Starting Mobility Calculation" << std::endl;

    for (auto &pool: *V.GetEntityList()) {
        logstream.information() << pool->GetName() << std::endl;
        logstream.information() << pool->GetSize() << std::endl;
        if (pool->GetName() == "Mobile Entity") {
            auto *ME = dynamic_cast<MobileEntity *>(pool.get());
            for (size_t i = 0; i < pool->GetSize(); i++) {
                logstream.information() << "START POSITION : " << ME->DP().GetX(i) << std::endl;
            }
        }
    }
    logstream.information() << ">> 8 | Found Total Entities: " << V.GetEntityCount() << std::endl;
    for (auto &pool: *V.GetEntityList()) {
        logstream.information() << ">> 8 | " << pool->GetName() << std::endl;
    }
    logstream.information() << "....................................." << std::endl;

    double dt = 1.0;

    Eigen::ArrayXd X;
    Eigen::ArrayXd VX;
    Eigen::ArrayXd Y;
    Eigen::ArrayXd VY;
    Eigen::ArrayXd sigma;
    size_t size;

    RandomVar1D Xdist(m_MacroScenarios);

    for (auto &et: *V.GetEntityList()) {

        if (et->GetName() == "Mobile Entity") {
            size = et->GetSize();

            auto *ME = dynamic_cast<MobileEntity *>(et.get());
            X = ME->DP().GetX();
            Y = ME->DP().GetY();
            VX = ME->DP().GetVX();
            VY = ME->DP().GetVY();
            sigma = ME->sigma().GetSigma();
        }
    }

    /**
     *
     *    SYSTEM INVOCATION
     *
     */

    for (int s = 0; s < m_MacroScenarios; s++) {
        for (int steps = 0; steps < m_CalculationHorizon; steps++) {
            compute_movement(X, VX, Y, VY, sigma, dt, logstream);
        }
    }

}