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

#include "model_data.h"
#include "FactorLoadings.h"
#include "CorrelationMatrix.h"
#include "TransitionMatrix.h"
#include "ARProcess.h"
#include "CreditCurve.h"
#include "HazardRate.h"
#include "TransitionThresholds.h"
#include "LGDModel.h"
#include "MarkovChain.h"

Model_Data::Model_Data(Simulation &W, Model &M, Poco::LogStream &logstream) {

    int model_family = M.ModelFamilyID();
    logstream.information() << "> Creating Model Components for Model Family " << M.ModelFamilyName(model_family) << std::endl;

    switch (model_family) {
        case 0:
            // Not required
            break;
        case 14:
            m_Model_Components.emplace_back(std::make_shared<ARProcess>(W, M, logstream));
            m_Model_Components.emplace_back(std::make_shared<CorrelationMatrix>(W, M, logstream));
            break;
        default:
            logstream.error() << "> ERROR: Invalid Model Family" << std::endl;
            abort();
    }

    // m_Debug = true;
}


