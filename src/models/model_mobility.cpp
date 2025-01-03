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

#include "stats.hpp"

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

    stats::rand_engine_t engine(123);

    for (auto &pool: *V.GetEntityList()) {
        logstream.information() << ">> 8 | Found Total Entities: " << V.GetEntityCount() << std::endl;
        logstream.information() << pool->GetName() << std::endl;
        logstream.information() << pool->GetSize() << std::endl;
        if (pool->GetName() == ">> 8 | Mobile Entities: ") {
            auto *ME = dynamic_cast<MobileEntity *>(pool.get());
            for (size_t i = 0; i < pool->GetSize(); i++) {
                logstream.information() << ">> 8 | START X POSITIONS : " << ME->DP().GetX(i) << std::endl;
            }
        }
    }

    /**
     *
     *  INITIALIZATION
     *
     */

    double dt = 1.0;

    Eigen::ArrayXd X;
    Eigen::ArrayXd VX;
    Eigen::ArrayXd Y;
    Eigen::ArrayXd VY;
    Eigen::ArrayXd sigma;

    for (auto &et: *V.GetEntityList()) {
        if (et->GetName() == "Mobile Entity") {
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
     *  SYSTEM INVOCATION
     *
     */
    RandomVar R(m_MacroScenarios, 1);

    for (int scenarios = 0; scenarios < m_MacroScenarios; scenarios++) {
        for (int steps = 0; steps < m_CalculationHorizon; steps++) {
            compute_movement(X, VX, Y, VY, sigma, dt, engine, logstream);
        }
        R.setS(scenarios, X[0]);
    }

    /**
     *  RESULT STORAGE
     */
    for (auto &rc: *MR.GetResultComponent()) {
        logstream.information() << rc->GetName() << std::endl;
        if (rc->GetName() == "Random Var 1D") {
            auto *RV1D = dynamic_cast<RandomVar1D *>(rc.get());
            RV1D->SetRV(R);
        }
    }
}