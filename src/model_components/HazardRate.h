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


#include "model_component.h"

/**
 * Hazard Rate Model Component.
 */
class HazardRate : public ModelComponent {
public:

    HazardRate(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Hazard Rate")  {
        std::string fragment = "HazardRate";
        Initialize(fragment, W, M, logstream);
    }

    ~HazardRate() override = default;

    void Deserialize(){};

    Eigen::VectorXd Hazard() {
        return hazard;
    }

private:
    Eigen::VectorXd hazard; // store hazard rate shape
};