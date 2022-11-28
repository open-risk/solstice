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


#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <Poco/ConsoleChannel.h>
#include "core/simulation.h"

TEST_CASE("Solstice logger can be instantiated", "[engine]") {

    std::cout << "------------------------------------" << std::endl;
    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);
    Poco::Logger::root().setChannel(pCons);
    Poco::Logger &logger = Poco::Logger::get("Test_Logger");
    logger.setLevel(LOG_LEVEL);
    Poco::LogStream logstream(logger);
    logstream << "TEST" << std::endl;

}