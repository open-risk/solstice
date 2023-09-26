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

#include <Poco/SimpleFileChannel.h>
#include "core/simulation.h"

Simulation::Simulation(int operating_mode, std::string& payload, Poco::LogStream &logstream, char** arg_strings)
: m_logstream(logstream) {

    m_sim_operating_mode = operating_mode;
    char *config_name = arg_strings[1];

    std::string c_time = get_current_time();
    std::string path = Poco::Path::current();

    logstream.information() << IdentityLog(arg_strings[0]);

    if(m_RunLevel == 0) {
        abort();
    }

    // Read configuration data
    if (operating_mode == 0) {

        // Override hardwired configuration with command line config file
        if (config_name) {
            std::string s(config_name);
            m_configuration_file = s;
        }
        else  {
            m_configuration_file = path + m_configuration_dir + m_configuration_file;
        }
        logstream.information() << "> Reading Configuration filename: " + m_configuration_file << std::endl;
        ReadFromFile(m_configuration_file, logstream);
        logstream.information() << "> Configuration Description: " + this->Description() << std::endl;
        logstream.information() << "> New Verbosity Level: " << this->VerboseLevel() << std::endl;
    }
    else if (operating_mode == 3) {
        // BATCH MODE USING HARDWIRED CONFIGURATION
        m_configuration_file = path + "../../" + m_configuration_dir + m_configuration_file;
        logstream.information() << "> Reading Configuration filename: " + m_configuration_file << std::endl;
        ReadFromFile(m_configuration_file, logstream);
        logstream.information() << "> Configuration Description: " + this->Description() << std::endl;
        logstream.information() << "> Configuration Verbosity Level: " << this->VerboseLevel() << std::endl;
    } else if (operating_mode == 1) {
        logstream.information() << "> CGI Parsing Client HTTP Payload" << std::endl;
        logstream.information() << "> ---------------------------" << std::endl;
        for (const auto &i: ENV) {
            logstream.information() << i << " : ";
            // attempt to retrieve value of environment variable
            char *value = getenv(i.c_str());
            if (value != nullptr) {
                logstream.information() << value << std::endl;
            } else {
                logstream.error() << "EMPTY" << std::endl;
            }
        }
        logstream.information() << "> ---------------------------" << std::endl;
        // Copy all data from cin, using iterators.
        std::istream_iterator<char> begin(std::cin >> std::noskipws);
        std::istream_iterator<char> end;
        std::string json(begin, end);
        // Store the received json std::string for further use
        input_json_string = json;
        logstream.information() << "> Hardwired Verbosity Level: " << LOG_LEVEL << std::endl;
        this->Deserialize(json, logstream);
        logstream.information() << "> Configuration Description: " + this->Description() << std::endl;
        logstream.information() << "> Configuration Run Level: " + std::to_string(this->RunLevel()) << std::endl;
    }
    else if (operating_mode == 2) {
        logstream.information() << "> Server Parsing Client HTTP Payload" << std::endl;
        logstream.information() << "> ---------------------------" << std::endl;
        for (const auto &i: ENV) {
            logstream.information() << i << " : ";
            // attempt to retrieve value of environment variable
            char *value = getenv(i.c_str());
            if (value != nullptr) {
                logstream.information() << value << std::endl;
            } else {
                logstream.error() << "EMPTY" << std::endl;
            }
        }
        logstream.information() << "> ---------------------------" << std::endl;
        // Copy all data from cin, using iterators.
//        std::istream_iterator<char> begin(std::cin >> std::noskipws);
//        std::istream_iterator<char> end;
//        std::string json(begin, end);
        // Store the received json std::string for further use
        input_json_string = payload;
        logstream.information() << "> Hardwired Verbosity Level: " << LOG_LEVEL << std::endl;
        this->Deserialize(payload, logstream);
        logstream.information() << "> Configuration Description: " + this->Description() << std::endl;
        logstream.information() << "> Configuration Run Level: " + std::to_string(this->RunLevel()) << std::endl;
    }

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 1 | Configuration: Updating API URL's / Local Dir Paths" << std::endl;
    }

    if (operating_mode == 3 || operating_mode == 0) {
        logstream.information() << "> Read and Parsed Configuration Data from: " << m_configuration_file << std::endl;
    } else if (operating_mode == 1) {
        logstream.information() << "> Read and Parsed Configuration Data from API " << std::endl;
    }

    if (LOG_LEVEL >= 4) {
        this->Serialize(logstream);
    }
    if(m_RunLevel == 1) {
        exit(0);
    }
}

