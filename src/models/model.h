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

#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "core/simulation.h"
#include "networks/network.h"
#include "scenarios/scenarios.h"
#include "managers/managers.h"
#include "model_components/model_data.h"
#include "insights/insights.h"

class Insights;

class Managers;

class Model {
public:

    Model(Simulation &W, Poco::LogStream &logstream);

    ~Model() = default;

    void Deserialize(const std::string &json, Poco::LogStream &logstream, Simulation &W);

    void Print(Poco::LogStream &logstream);

    void Validate(Poco::LogStream &logstream);

    void
    Calculate(Network &V, Scenarios &S, Model_Data &MD, Managers &MA, Insights &MR, Poco::LogStream &logstream);

    void
    model_mobility(Network &Po, Scenarios &S, Model_Data &MD, Managers &MA, Insights &MR, Poco::LogStream &logstream);

    void
    model_test(Network &Po, Scenarios &S, Model_Data &MD, Managers &MA, Insights &MR, Poco::LogStream &logstream);

    void compute_movement(Eigen::ArrayXd &X, Eigen::ArrayXd &VX, Eigen::ArrayXd &Y, Eigen::ArrayXd &VY,
                          Eigen::ArrayXd &sigma, double dt, Poco::LogStream &logstream);

    [[nodiscard]] int ModelFamilyID() const {
        return m_ModelFamilyID;
    };

    [[nodiscard]] int RootScenarios() const {
        return m_PortfolioScenarios * m_MacroScenarios;
    };

    [[nodiscard]] int PortfolioScenarios() const {
        return m_PortfolioScenarios;
    };

    [[nodiscard]] int MacroScenarios() const {
        return m_MacroScenarios;
    };

    [[nodiscard]] int MacroSimulations() const {
        return m_MacroScenarios * m_BranchScenarios;
    };

    [[nodiscard]] int BranchScenarios() const {
        return m_BranchScenarios;
    };

    [[nodiscard]] int GetRiskHorizon() const {
        return m_RiskHorizon;
    };

    [[nodiscard]] int CalculationHorizon() const {
        return m_CalculationHorizon;
    };

    [[nodiscard]] int Interval() const {
        return m_Interval;
    }

    [[nodiscard]] int TimePoints() const {
        return m_CalculationHorizon + 1;
    };

    [[nodiscard]] int Resolution() const {
        return m_Resolution;
    };

    [[nodiscard]] int getLossGrid() const {
        return m_LossGrid;
    }

    [[nodiscard]] int GetLossSimulations() const { return Loss_Simulations; };

    int MacroSimulations() { return Macro_Simulations; };

    int ScenarioMethod() const {
        return m_ScenarioMethod;
    };

    int GetMaturityMethod() const {
        return m_MaturityMethod;
    }

private:

    int m_AnalyticTests{};

    std::map<std::string, std::function<void(Network &Po, Scenarios &S, Model_Data &MD, Managers &MA,
                                             Insights &MR, Poco::LogStream &logstream)>> ModelMap;

    int m_ModelFamilyID{};

    bool m_DefaultOnly{};

    int m_Markov_Flag{};

    int m_ScenarioMethod{};

    int m_MaturityMethod{};
    int Loss_Simulations = 1;
    int Total_Simulations = 1;
    int Macro_Simulations = 1;
    int m_LossGrid{};

    int m_PortfolioScenarios{};
    int m_BranchScenarios{};
    int m_MacroScenarios{};

    int m_CalculationHorizon{};

    int m_Interval{};

    int m_RiskHorizon{};

    int m_Resolution{};
    int m_CDP_Method{};

    bool m_Debug{};

};

#endif
