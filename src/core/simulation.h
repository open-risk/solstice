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

#pragma once

#ifndef SOLSTICE_SIMULATION_H
#define SOLSTICE_SIMULATION_H

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#define my_stringify(x) stringify_literal( x )
#define stringify_literal(x) # x

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <iterator>
#include <ctime>
#include <string>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>

#include "utils/utils.h"
#include "utils/colors.h"

const std::string ENV[26] = {
        "COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",
        "HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",
        "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",
        "HTTP_HOST", "HTTP_USER_AGENT", "PATH",
        "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",
        "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_filename",
        "SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",
        "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL",
        "SERVER_SIGNATURE", "SERVER_SOFTWARE", "CONTENT_TYPE",
        "CONTENT_LENGTH"
};

std::string get_current_time();

/**
 * @brief Simulation is a self-contained Solstice calculation.
 *
 *
 */
class Simulation {

public:

    Simulation(int operating_mode, std::string &payload, Poco::LogStream &logstream, char **arg_strings);

    ~Simulation() = default;

    /**
     * Read a Solstice configuration object from file.
     * @param filename
     * @param logstream
     */
    void ReadFromFile(const std::string &filename, Poco::LogStream &logstream);

    /**
     *
     * @param json
     * @param logstream
     */
    void Deserialize(const std::string &json, Poco::LogStream &logstream);

    void Serialize(Poco::LogStream &logstream);

    void Validate(Poco::LogStream &logstream);

    int run(Poco::LogStream &logstream);

    void CGI_control(int level) const;

    std::string IdentityLog(char *command_name);

    void control_point(int level, Poco::LogStream &logstream);

    void
    announce_level(int step, const std::string &step_string, int verbose_level, Poco::LogStream &logstream,
                   int operating_mode);

    int getLogging() {
        return m_logging;
    }

    const int getSummary_output() {
        return m_summary_output;
    }

    [[nodiscard]] int getPath_mode() const {
        return m_path_mode;
    }

    [[nodiscard]] int getOperating_mode() const {
        return m_sim_operating_mode;
    }

    [[nodiscard]] const std::string *getOperating_modes() const {
        return m_sim_operating_modes;
    }

    [[nodiscard]] int RunLevel() const {
        return m_RunLevel;
    };

    [[nodiscard]] const std::string &getConfigurationfile() const {
        return m_configuration_file;
    }

    std::string getToken() {
        return m_APItoken;
    }

    void setDebug_mode(int i);

    // General Data

    [[nodiscard]] std::string ID() const {
        return m_WorkflowID;
    };

    [[nodiscard]] std::string Description() const {
        return m_Description;
    };

    [[nodiscard]] std::string InputString() const {
        return input_json_string;
    }

    [[nodiscard]] std::string ConfigurationFile() const {
        return m_configuration_file;
    }

    [[nodiscard]] int NetworkShape() const {
        return m_NetworkShape;
    };

    [[nodiscard]] std::string BaseAPIURL() const {
        return m_BaseAPIURL;
    };

    [[nodiscard]] std::string DataDir() const {
        return m_DataDir;
    };

    [[nodiscard]] std::string PortfolioURL() const {
        return m_PortfolioURL;
    };

    void SetPortfolioURL(std::string PortfolioURL) {
        m_PortfolioURL = std::move(PortfolioURL);
    };

    [[nodiscard]] std::string CorrelationURL() const {
        return m_CorrelationURL;
    };

    [[nodiscard]] std::string LoadingsURL() const {
        return m_LoadingsURL;
    };

    std::string ARProcessURL() {
        return m_ARProcessURL;
    }

    [[nodiscard]] std::string LiabilitiesURL() const {
        return m_LiabilitiesURL;
    };

    [[nodiscard]] std::string ThresholdDataURL() const {
        return m_ThresholdDataURL;
    };

    [[nodiscard]] std::string TransitionDataURL() const {
        return m_TransitionDataURL;
    };

    [[nodiscard]] std::string CreditCurveURL() const {
        return m_CreditCurveURL;
    };

    [[nodiscard]] std::string HazardRateURL() const {
        return m_HazardRateURL;
    };

    [[nodiscard]] std::string ScenarioDataURL() const {
        return m_ScenarioDataURL;
    };

    [[nodiscard]] std::string AdjacencyURL() const {
        return m_AdjacencyURL;
    };

    [[nodiscard]] std::string LGDModelURL() const {
        return m_LGDModelURL;
    };

    [[nodiscard]] std::string MarkovChainURL() const {
        return m_MarkovChainURL;
    };

    [[nodiscard]] std::string ResultsURL() const {
        return m_ResultsURL;
    };

    static int VerboseLevel() {
        return LOG_LEVEL;
    };

    [[nodiscard]] int ReportingMode() const {
        return m_ReportingMode;
    };

    [[nodiscard]] Eigen::VectorXi ResultsList() const {
        return m_ResultsList;
    };

    [[nodiscard]] int LiabilityDataMode() const {
        return m_LiabilityDataMode;
    };

    [[nodiscard]] int ScenarioDataMode() const {
        return m_ScenarioDataMode;
    };


private:

    int m_sim_operating_mode = 0;
    std::string m_sim_operating_modes[4] = {"Interactive TTY", "CGI",
                                            "HTTP", "Batch"};
    int const static m_logging = 0;
    int const static m_summary_output = 0;

    Poco::LogStream &m_logstream;

    size_t m_RunLevel = 1;
    std::string m_configuration_dir = "Data/Shared/Configurations/";
    std::string m_configuration_file = "workflow_data_0_DRYRUN.json";
    std::string m_APItoken = "";
    std::string input_json_string;
    std::string m_Description;
    std::string m_WorkflowID{};
    std::string m_WorkflowName{};
    int m_NetworkShape{};
    int const m_path_mode = 1;
    std::string m_BaseAPIURL;
    std::string m_DataDir;
    int m_LiabilityDataMode{};
    int m_ScenarioDataMode{};
    std::string m_PortfolioURL;
    std::string m_LiabilitiesURL;
    std::string m_AdjacencyURL;
    std::string m_ScenarioDataURL;
    std::string m_CorrelationURL;
    std::string m_LoadingsURL;
    std::string m_ThresholdDataURL;
    std::string m_TransitionDataURL;
    std::string m_ARProcessURL;
    std::string m_CreditCurveURL;
    std::string m_HazardRateURL;
    std::string m_LGDModelURL;
    std::string m_MarkovChainURL;
    std::string m_ResultsURL;
    int m_ReportingMode{};
    Eigen::VectorXi m_ResultsList;
    Eigen::MatrixXi Report_Matrix;
    int const Stages = 3;
    int m_Verbose_Level = 2;
    bool m_Debug = DEBUG_MODE;

};

#endif /* SOLSTICE_SIMULATION_H */


