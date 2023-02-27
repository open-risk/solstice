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

#include "models/model.h"

void Model::compute_movement(Eigen::ArrayXd &X,
                             Eigen::ArrayXd &VX,
                             Eigen::ArrayXd &Y,
                             Eigen::ArrayXd &VY,
                             Eigen::ArrayXd &sigma,
                             double dt,
                             stats::rand_engine_t &engine,
                             Poco::LogStream &logstream) {

    /**
     * System Level Flags
     */

    // 0-> Gaussian Noise
    // 1-> Wiener Process (Random Walk)
    // 2-> Ornstein-Uhlenbeck process (Brownian Motion with Stochastic Velocity)
    int ProcessType = 0;

    // 0-> Uniform (-1, 1)
    // 1-> Normal
    int DistributionType = 1;

    size_t size = X.size();

    Eigen::ArrayXd DBX;
    Eigen::ArrayXd DBY;

    if (DistributionType == 0) {
        DBX = Eigen::ArrayXd::Random(size);
        DBY = Eigen::ArrayXd::Random(size);
    } else if (DistributionType == 1) {
        DBX.resize(size);
        DBY.resize(size);
        for (int i = 0; i < size; i++) {
            DBX[i] = stats::rnorm(0.0, 1.0, engine);
            DBY[i] = stats::rnorm(0.0, 1.0, engine);
        }
    }

    if (ProcessType == 0) {
        X = DBX;
        Y = DBY;
    } else if (ProcessType == 1) {
        X += DBX;
        Y += DBY;
    } else if (ProcessType == 2) {
        X += sigma * VX * dt;
        Y += sigma * VY * dt;
        VX += DBX;
        VY += DBY;
    }
}

