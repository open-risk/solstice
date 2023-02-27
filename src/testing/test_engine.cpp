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

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <Poco/ConsoleChannel.h>
#include "core/simulation.h"

TEST_CASE("Solstice can be instantiated", "[engine]") {

    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);
    Poco::Logger::root().setChannel(pCons);
    Poco::Logger &logger = Poco::Logger::get("Test_Logger");
    logger.setLevel(LOG_LEVEL);
    Poco::LogStream logstream(logger);
    std::string path = Poco::Path::current();
    std::string configuration_dir = "Data/Local/Configurations/";
    std::string configuration_file = "workflow_data_0_DRYRUN.json";
    configuration_file = path + "../" + configuration_dir + configuration_file;
    std::string exec_name = "Catch2 Test";
    char** argv[2];
    argv[0] = (char **) exec_name.c_str();
    argv[1] = (char **) configuration_file.c_str();
    Simulation engine(OPERATING_MODE, logstream, reinterpret_cast<char **>(argv));
    std::cout << "DONE" << std::endl;
//    INFO("Test");
}