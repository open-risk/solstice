/**
 * general API for macro scenario data
 * macro scenario data generators
 * File:   scenarios.cpp
 * Author: Open Risk
 *
 * Applies only when fetching existing scenario data
 * Simulated scenarios are produced as part of model calculations (preliminary step)
 */

#include "scenarios.h"

Scenarios::Scenarios(Simulation &W, Model &M, Poco::LogStream &logstream) {

    if (W.ScenarioDataMode() == 0) {
        logstream.information() << "> No macro scenario data required in this Solstice configuration" << std::endl;
    } else {

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 1 | Fetching Scenario Data from API URL's / Local Dir Paths" << std::endl;
        }
        Initialize(W, M, logstream);
    }

}

