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

#pragma once

#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "core/simulation.h"
#include "models/model.h"

class Model_Data;

class Model;

class Scenarios {
public:

    Scenarios(Simulation &W, Model &M, Poco::LogStream &logstream);

    ~Scenarios() = default;

    void Initialize(Simulation &W, Model &M, Poco::LogStream &logstream);

    void ReadFromFile(std::string filename, Simulation &W, Model &M,  Poco::LogStream &logstream);

    void Deserialize(const std::string& json, Simulation &W, Model &M, Poco::LogStream &logstream);

    void GetFromAPI(std::string &URL, Simulation &W, Model &M, Poco::LogStream &logstream);

    void Serialize(int Level, Model &M, Poco::LogStream &logstream);

    void Print(Poco::LogStream &logstream);

    void Validate(Poco::LogStream &logstream, Model &M);

    Eigen::Tensor<double, 3> GetMacroScenarios() { return Macro_Scenarios; };

    void Print(int Level, Model &M, Poco::LogStream &logstream);

    int GetFactorScenarios() {
        return m_FactorScenarios;
    }

    int GetFactors() {
        return m_Factors;
    }

    void SetFactors(int Factors) {
        m_Factors = Factors;
    }

    Eigen::MatrixXd GetFactorData() {
        return m_FactorData;
    }

    Eigen::Tensor<double, 3> GetZ3() {
        return z3;
    }

    Eigen::VectorXd GetZ1() {
        return z1;
    }

    void SetMacroScenarios(Eigen::Tensor<double, 3> &V) { Macro_Scenarios = V; };

    Eigen::VectorXd GetProbabilities() {
        return m_FactorProbabilities;
    }

    int ScenarioPeriods() const {
        return m_ScenarioPeriods;
    };

private:

    int m_ScenarioPeriods{};

    int m_FactorScenarios{};
    int m_Factors{};

    Eigen::MatrixXd m_FactorData;
    Eigen::VectorXd m_FactorProbabilities;

    Eigen::Tensor<double, 3> z3;

    Eigen::VectorXd z1;

    Eigen::Tensor<double, 3> Macro_Scenarios;

    bool m_Debug{};
};

#endif
