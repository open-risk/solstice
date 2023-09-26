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

#include <iostream>
#include <Poco/SimpleFileChannel.h>
#include <Poco/ConsoleChannel.h>
#include "Poco/Environment.h"
#include "Poco/Path.h"
#include "core/simulation.h"

int main(int num_args, char **arg_strings) {

    //
    // Start logging
    //

    // Setup console and file log channels
    Poco::AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);

    // Construct path to logfile
    std::string logstreamname;

    // Add Timestamp to log file
    // If running in debug mode overwrite previous configuration file
    // Else produce a timestamp
    if (OPERATING_MODE == 3) {
        logstreamname = "../../Logs/configuration.log";
    } else {
        logstreamname = "Logs/configuration_" + get_current_time() + ".log";
    }

    std::string path = Poco::Path::current();

    if (OPERATING_MODE == 1 || OPERATING_MODE == 3) {
        // Configure file logging
        // Must have Log directory writeable
        pChannel->setProperty("path", path + logstreamname);
        pChannel->setProperty("rotation", "never");
        try {
            Poco::Logger::root().setChannel(pChannel);
        }
        catch (...) {
            if (OPERATING_MODE == 1) {
                std::cout << "Content-Type: application/json\n\n";
                std::cout << "ERROR: Could not open log file" << std::endl;
            } else if (OPERATING_MODE == 3) {
                std::cout << "ERROR: Could not open log file" << std::endl;
            }
            abort();
        }
    }// If interactive, send messages to std::cout
    else if (OPERATING_MODE == 0) {
        Poco::Logger::root().setChannel(pCons);
    }
    Poco::Logger &logger = Poco::Logger::get("Configuration_Logger"); // inherits root channel

    // Mapping verbosity levels to Poco's log levels
    //> 0 PRIO_FATAL (highest priority)
    //> 1 PRIO_CRITICAL
    //> 2 PRIO_ERROR
    //> 3 PRIO_WARNING
    //> 4 PRIO_NOTICE
    //> 5 PRIO_INFORMATION
    //> 6 PRIO_DEBUG
    //> 7 PRIO_TRACE (lowest priority)

    logger.setLevel(LOG_LEVEL);

//    logger.fatal("0 FATAL");
//    logger.critical("1 CRITICAL");
//    logger.error("2 ERROR");
//    logger.warning("3 WARNING");
//    logger.notice("4 NOTICE");
//    logger.information("5 INFORMATION");
//    logger.debug("6 DEBUG");
//    logger.trace("7 TRACE");

    Poco::LogStream logstream(logger);

    //> 0 PRIO_FATAL (highest priority)
    //> 1 PRIO_CRITICAL
    //> 2 PRIO_ERROR
    //> 3 PRIO_WARNING
    //> 4 PRIO_NOTICE
    //> 5 PRIO_INFORMATION
    //> 6 PRIO_DEBUG
    //> 7 PRIO_TRACE (lowest priority)

    // Create a Solstice simulation instance
    // Instantiation provides a few global hard-coded options
    // Reads general config parameters from JSON file
    std::string payload = "Debug";
    Simulation engine(OPERATING_MODE, payload, logstream, arg_strings);

    // Start running the engine
    engine.run(logstream);

    // Shutdown logging system
    Poco::Logger::shutdown();

    return 0;
}


