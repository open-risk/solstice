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

#include "insights.h"
#include "utils/random_var.h"

void Insights::reporting_message(int level, const std::string &Message, Poco::LogStream &logstream) {
    if (level >= 1) {
        logstream.warning() << ">> 9 | " << Message << std::endl;
    }
}

void Insights::Report(Simulation &W, Model &M, Scenarios &S, Model_Data &D, Network &P, Managers &MA,
                         Poco::LogStream &logstream) {

    int operating_mode = W.getOperating_mode();
    std::string filename = W.ResultsURL();
    if (W.getPath_mode() == 1) {
        filename = W.DataDir() + "/" + filename;
        logstream.information() << "DATA DIR " << W.DataDir() << std::endl;
        logstream.information() << "RESULTS FILE " << filename << std::endl;
    }
    std::ofstream resultsJSON(filename);

    Poco::JSON::Object model_output;
    Poco::JSON::Array results_data;
    Poco::JSON::Object logstream_data;

    std::stringstream results;

    Eigen::VectorXi result_list = W.ResultsList();
    int result_no = result_list.size();
    logstream.warning() << "---------------------------------" << std::endl;
    logstream.warning() << ">> 9 | Number of Results: " << result_no << std::endl;
    logstream.warning() << "---------------------------------" << std::endl;

    for (int i = 0; i < result_no; i++) {
        add_result_object(result_list[i], results_data, W, P, M, D, MA, S, logstream);
        if (LOG_LEVEL >= 1) {
            logstream.warning() << "---------------------------------" << std::endl;
            logstream.warning() << ">> 9 | Added Result: " << result_list[i] << std::endl;
            logstream.warning() << "---------------------------------" << std::endl;
        }
    }

    std::stringstream buffer;
    buffer << logstream.rdbuf();

    model_output.set("results_data", results_data);
    model_output.set("logstream", buffer.str());
    Poco::JSON::Stringifier::stringify(model_output, results);
    if (LOG_LEVEL >= 3) {
        logstream.warning() << "---------------------------------" << std::endl;
        logstream.warning() << ">> 9 | Results: " << results.str() << std::endl;
    }

    if (operating_mode == 1 || operating_mode == 2) {
        logstream.warning() << ">> 9 | Sending JSON Results " << std::endl;
        std::cout << "Content-Type: application/json\n\n";
        std::cout << results.str();
    }
    else if (operating_mode == 0 || operating_mode == 3) {
        resultsJSON << results.str();
        resultsJSON.close();
        if (LOG_LEVEL >= 1) {
            logstream.warning() << "---------------------------------" << std::endl;
            logstream.warning() << ">> 9 | JSON Results: " << filename << std::endl;
        }
    } else {
        std::cout << "ERROR: Invalid operating mode for output" << std::endl;
    }
}


