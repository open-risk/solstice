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

#include "insights.h"
#include "RandomVar1D.h"

Insights::Insights(Simulation &W, Model &M, Network &P, Poco::LogStream &logstream) {

    this->Deserialize(W.InputString(), logstream);
    logstream.information() << "> Insights Configuration Parsed from Configuration Object" << std::endl;

    if (M.ModelFamilyID() == 0) {

    } else if (M.ModelFamilyID() == 1) {
        size_t size = M.CalculationHorizon();
    } else if (M.ModelFamilyID() == 14) {
        size_t size = M.MacroScenarios();
        m_Result_Components.emplace_back(std::make_shared<RandomVar1D>(size));
    } else {
        logstream.error() << "> ERROR: Invalid Model Family" << std::endl;
        abort();
    }
}
