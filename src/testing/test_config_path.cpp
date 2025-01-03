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

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <filesystem>
#include "utils/utils.h"

TEST_CASE("Solstice config is accessible", "[config]") {

    std::string path = Poco::Path::current();
    Poco::Path root_path(path);
    root_path.makeParent();

    // std::string path = Poco::Path::parent();
    std::string configuration_dir = "../Data/Local/Configurations/";
    std::string configuration_file = "workflow_data_0_DRYRUN.json";
    // configuration_file = path + "../" + configuration_dir + configuration_file;
    configuration_file = root_path.toString() + configuration_dir + configuration_file;
    std::cout << configuration_file << std::endl;
    std::string log_dir = "../Logs/";
    log_dir = root_path.toString() + log_dir;
    std::string log_file = "configuration.log";
    log_file = log_dir + log_file;
    std::cout << log_dir << std::endl;
    std::cout << log_file << std::endl;

//    INFO("Config Dir: " << configuration_dir);
//    INFO("Config File: " << configuration_file);
//    INFO("Current Path: " << path);
//    INFO("Log Dir: " << log_dir);
    REQUIRE(std::filesystem::exists(configuration_file));
    REQUIRE(std::filesystem::exists(log_dir));
}