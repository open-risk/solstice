/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
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

std::string Simulation::IdentityLog(char *command_name) {

    std::string OS_Name = Poco::Environment::osName();
    std::string OS_Version = Poco::Environment::osVersion();
    std::string OS_Arch = Poco::Environment::osArchitecture();
    std::string OS_Node_Name = Poco::Environment::nodeName();
    std::string OS_ID = Poco::Environment::nodeId();;

    unsigned int x = Poco::Environment::libraryVersion();
    std::stringstream ss;
    ss << std::hex << x;
    std::string poco_version = ss.str();

    ss.str("");
    ss << EIGEN_MAJOR_VERSION;
    ss << ".";
    ss << EIGEN_MINOR_VERSION;
    std::string eigen_version = ss.str();
    std::string solstice_version = my_stringify(VERSION);

    ss.str("");
    ss << this->m_sim_operating_mode;
    ss << " (";
    ss << this->m_sim_operating_modes[this->m_sim_operating_mode];
    ss << ")";
    std::string operating_mode = ss.str();

    std::string verbosity_level = std::to_string(LOG_LEVEL);

    std::string path_mode;
    if (this->m_path_mode == 0) {
        path_mode = "Absolute Path";
    } else if (this->m_path_mode == 1) {
        path_mode = "Relative Path";
    };

    std::string current_path = Poco::Path::current();
    std::string s(command_name);
    std::string logheader;
    logheader = logheader + "> Starting solstice Execution at " + get_current_time() + "\n";
    logheader = logheader + "> Operating System: " + OS_Name + "\n";
    logheader = logheader + "> OS Version: " + OS_Version + "\n";
    logheader = logheader + "> OS Architecture: " + OS_Arch + "\n";
    logheader = logheader + "> Node Name: " + OS_Node_Name + "\n";
    logheader = logheader + "> Node ID: " + OS_ID + "\n";
    logheader = logheader + "> Poco Version: " + poco_version + "\n";
    logheader = logheader + "> Eigen Version: " + eigen_version + "\n";
    logheader = logheader + "> Solstice Executable: " + s + "\n";
    logheader = logheader + "> Solstice Version: " + solstice_version + "\n";
    logheader = logheader + "> Operating Mode: " + operating_mode + "\n";
    logheader = logheader + "> Hardwired Verbosity Level: " + verbosity_level + "\n";
    logheader = logheader + "> Path Mode: " + path_mode + "\n";
    logheader = logheader + "> Current Path: " + current_path + "\n";

    return logheader;

}
