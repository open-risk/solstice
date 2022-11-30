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

#include "simulation.h"

void Simulation::Serialize(Poco::LogStream &logstream) {

    logstream.information() << ">> 1 | Configuration Data Summary " << std::endl;

    std::string run_levels[11] = {"Environment", "Initialization", "Configuration",
                                  "Model Configuration", "Model Data", "Scenario Data", "Entity Data", "Manager Data",
                                  "Diagnostic Tests", "Model Calculation", "Reporting"};

    logstream.information() << ">> 1 | -------- Begin Configuration Description --------" << std::endl;
    logstream.information() << ">> 1 | ID: " << m_WorkflowID << std::endl;
    logstream.information() << ">> 1 | Description: " << m_Description << std::endl;
    logstream.information() << ">> 1 | API Base: " << m_BaseAPIURL << std::endl;
    logstream.information() << ">> 1 | Root Dir: " << m_DataDir << std::endl;
    logstream.information() << ">> 1 | Debug Mode: " << m_Debug << std::endl;
    logstream.information() << ">> 1 | Run Level: " << run_levels[m_RunLevel] << std::endl;
    logstream.information() << ">> 1 | Log Level: " << LOG_LEVEL << std::endl;
    logstream.information() << ">> 1 | -------- Input Set  ---------" << std::endl;
    logstream.information() << ">> 1 | Liability Data Mode: " << m_LiabilityDataMode << std::endl;
    logstream.information() << ">> 1 | Scenario Data Mode: " << m_ScenarioDataMode << std::endl;
    logstream.information() << ">> 1 | Network URL: " << m_PortfolioURL << std::endl;
    logstream.information() << ">> 1 | Liabilities URL: " << m_LiabilitiesURL << std::endl;
    logstream.information() << ">> 1 | Correlation Matrix URL: " << m_CorrelationURL << std::endl;
    logstream.information() << ">> 1 | Adjacency Matrix URL: " << m_AdjacencyURL << std::endl;
    logstream.information() << ">> 1 | Loadings URL: " << m_LoadingsURL << std::endl;
    logstream.information() << ">> 1 | Threshold Data URL: " << m_ThresholdDataURL << std::endl;
    logstream.information() << ">> 1 | Transition Data URL: " << m_TransitionDataURL << std::endl;
    logstream.information() << ">> 1 | Scenario Data URL: " << m_ScenarioDataURL << std::endl;
    logstream.information() << ">> 1 | -------- Output Set ----------" << std::endl;
    logstream.information() << ">> 1 | Results URL: " << m_ResultsURL << std::endl;
    logstream.information() << ">> 1 | Reporting Mode: " << m_ReportingMode << std::endl;
    logstream.information() << ">> 1 | Results List: " << std::endl;
    logstream.information() << m_ResultsList;
    logstream.information() << ">> 1 | -------- End Configuration Description --------" << std::endl;
}