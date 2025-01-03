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

#include "core/simulation.h"
#include "models/model.h"

int Simulation::run(Poco::LogStream &logstream) {

    // Simulation has a logstream that is passed to all functions
    int step = 1;
    std::string step_string = "Starting Run Sequence";
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 2 (REQUIRED)
    // Initialize a Model (configuration) object (Generic)
    /*
    * CONFIGURATION STEP: Instantiate model configuration and read Model Configuration
    */
    step += 1;
    step_string = "Read Model Configuration";
    Model MC(*this, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 3 (REQUIRED)
    // Initialize Model Data Components (Model Family Choice)
    step += 1;
    step_string = "Read Model Data Components";
    Model_Data MD(*this, MC, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 4 (OPTIONAL)
    // Initialize Scenario Data Components (Scenario Data Mode Choice)
    step += 1;
    step_string = "Generate or Read Scenario Data";
    Scenarios S(*this, MC, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 5 (REQUIRED)
    // Initialize Entity-Component-Relations System (Network Shape Choice)
    step += 1;
    step_string = "Generate or Read Entity data";
    Network V(*this, MC, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 6 (OPTIONAL)
    step += 1;
    step_string = "Generate or Read Liability (Manager) data";
    // Initialize Managers (Manager Data Mode Choice)
    Managers MA(*this, MC, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 7 (REQUIRED)
    // Initialize Insights / Model Results (Model Family and Network Dependent - Output Settings)
    step += 1;
    step_string = "Create Model Results Storage";
    Insights MR(*this, MC, V, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 8 (OPTIONAL)
    // TODO Diagnostic Calculations (Model Family Dependent)
    step += 1;
    step_string = "Calculate Diagnostics";
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 9
    // Model Calculation (Model Family Choice)
    // Variations of Optional Scenarios: with Managers, Scenarios etc
    step += 1;
    step_string = "Start Model Calculation in Model Family " + MC.ModelFamilyName(MC.ModelFamilyID()) ;
    MC.Calculate(V, S, MD, MA, MR, logstream);
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);

    // RUN LEVEL 10
    // TODO Global Insights Reporting (From Cumulated Data)
    step += 1;
    step_string = "Reporting Results";
    announce_level(step, step_string, this->VerboseLevel(), logstream, m_sim_operating_mode);


    return 0;
};