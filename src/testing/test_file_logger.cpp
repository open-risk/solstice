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


#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <Poco/SimpleFileChannel.h>
#include "utils/utils.h"

TEST_CASE("Solstice file logger can be instantiated", "[logger]") {

    std::string path = Poco::Path::current();
    std::string log_dir = "../Logs/";
    log_dir = path + "../" + log_dir;
    std::string log_file = "configuration.log";
    log_file = log_dir + log_file;

    Poco::AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
    pChannel->setProperty("path", log_file);
    pChannel->setProperty("rotation", "never");
    Poco::Logger::root().setChannel(pChannel);
    Poco::Logger &logger = Poco::Logger::get("Test_Logger");
    logger.setLevel(LOG_LEVEL);
    Poco::LogStream logstream(logger);
    logstream.critical() << "Critical" << std::endl;
    logstream.error() << "Error" << std::endl;
    logstream.warning() << "Warning" << std::endl;
    logstream.notice() << "Notice" << std::endl;
    logstream.information() << "Information" << std::endl;
    logstream.debug() << "Debug" << std::endl;
    logstream.trace() << "Trace" << std::endl;
}