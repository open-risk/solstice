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

    int model_family = M.ModelFamilyID();
    logstream.information() << "> Creating Result Components for Model Family " << M.ModelFamilyName(model_family)
                            << std::endl;

    size_t size;

    switch (model_family) {
        case 0:
            // Not required
            break;
        case 9:
            size = M.MacroScenarios();
            m_Result_Components.emplace_back(std::make_shared<RandomVar1D>(size));
            break;
        case 14:
            size = M.MacroScenarios();
            m_Result_Components.emplace_back(std::make_shared<RandomVar1D>(size));
            break;
        default:
            logstream.error() << "> ERROR: Insights::Insights, Invalid Model Family" << std::endl;
            abort();
    }

}
