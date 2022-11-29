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

void Model::Calculate(Network &V, Scenarios &S, Model_Data &MD, Managers &MA, Insights &MR,
                      Poco::LogStream &logstream) {


    logstream.information() << "> Selecting Model Family: " << ModelFamilyName(m_ModelFamilyID) << std::endl;

    switch (this->ModelFamilyID()) {
        case 0:
            this->model_test(V, S, MD, MA, MR, logstream);
            break;
        case 14:
            this->model_mobility(V, S, MD, MA, MR, logstream);
            break;
        default:
            logstream.error() << "> ERROR: Invalid Model Family" << std::endl;
            abort();
    }

    logstream.information() << "------------------------------------------------------------------" << std::endl;
    logstream.information() << "> Finished Model Calculations of " << ModelFamilyName(m_ModelFamilyID) << std::endl;

}