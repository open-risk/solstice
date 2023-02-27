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

#ifndef Insights_H
#define Insights_H

#include "core/simulation.h"
#include "models/model.h"
#include "model_components/model_data.h"
#include "scenarios/scenarios.h"
#include "networks/network.h"
#include "managers/managers.h"
#include "result_component.h"

class Scenarios;

class Model;

class Simulation;

class Model_Data;

class Network;

class Managers;

class Insights {
public:

    Insights(Simulation &W, Model &M, Network &P, Poco::LogStream &logstream);

    ~Insights() = default;

    void Deserialize(std::string json, Poco::LogStream &logstream);

    auto GetResultComponent() {
        return &m_Result_Components;
    }

    static void reporting_message(int level, const std::string &Message, Poco::LogStream &logstream);

    void Report(Simulation &W, Model &M, Scenarios &S, Model_Data &D, Network &P, Managers &MA,
                Poco::LogStream &logstream);

    void
    add_result_object(int choice, Poco::JSON::Array &results_data, Simulation &W, Network &P, Model &M, Model_Data &MD,
                      Managers &MA, Scenarios &S, Poco::LogStream &logstream);

    [[nodiscard]] int ScenarioOutput() const {
        return m_ScenarioOutput;
    };

private:

    std::vector<std::shared_ptr<ResultComponent>> m_Result_Components;

    int m_ScenarioOutput{};

};

#endif /* Insights_H */


