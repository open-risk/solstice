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

void Model::Print(Poco::LogStream &logstream) {

    logstream.information() << ">> 2 | -------- Begin Model Configuration Description --------" << std::endl;
    logstream.information() << ">> 2 | Debug Mode: " << m_Debug << std::endl;
    logstream.information() << ">> 2 | Verbosity Level: " << LOG_LEVEL << std::endl;
    logstream.information() << ">> 2 | -------- Model Configuration --------" << std::endl;
    logstream.information() << ">> 2 | Model Family: " << "\t" << m_ModelFamilyID << std::endl;
    logstream.information() << ">> 2 | Number of Idiosyncratic Portfolio Scenarios:" << "\t" << m_PortfolioScenarios << std::endl;
    logstream.information() << ">> 2 | Number of Forward Expectation Scenarios:" << "\t" << m_BranchScenarios << std::endl;
    logstream.information() << ">> 2 | Calculation Horizon:" << "\t" << m_CalculationHorizon << std::endl;
    logstream.information() << ">> 2 | Risk Horizon:" << "\t" << m_RiskHorizon << std::endl;
    logstream.information() << ">> 2 | Interval:" << "\t" << m_Interval << std::endl;
    logstream.information() << ">> 2 | Factor Grid Resolution:" << "\t" << m_Resolution << std::endl;
    logstream.information() << ">> 2 | DefaultOnly:" << "\t" << m_DefaultOnly << std::endl;
    logstream.information() << ">> 4 | Scenario Method: " << "\t" << m_ScenarioMethod << std::endl;
}