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

#pragma once

#ifndef MANAGERS_H
#define MANAGERS_H

#include "core/simulation.h"
#include "models/model.h"
#include "entity_types/GenericEntity.h"

class Model;

class Managers : public GenericEntity {

public:

    Managers(Simulation &W, Model &M, Poco::LogStream &logstream) ;

    ~Managers() override = default;

    void Initialize(Simulation &W, Model &M, Poco::LogStream &logstream);

    void Deserialize(const std::string& json, Simulation &W, Poco::LogStream &logstream);

    void Validate(Poco::LogStream &logstream);

    void ReadFromFile(std::string filename, Simulation &W, Model &M, Poco::LogStream &logstream);

    void Print(Poco::LogStream &logstream) const;

    void SetDefaultRate(Eigen::MatrixXd &V) { Default_Rate = V; };

    [[nodiscard]] double GetInitialEquityCapital() const {
        return m_Initial_Equity_Capital;
    };

    Eigen::MatrixXd GetDefaultRate() { return Default_Rate; };

    [[nodiscard]] int LiabilityMethod() const {
        return m_LiabilityMethod;
    };

private:

    int m_size{};

    int m_LiabilityMethod{};

    double m_Initial_Equity_Capital{};

    Eigen::VectorXi m_manager_Tenor;
    Eigen::VectorXd m_manager_Attachment;
    Eigen::VectorXd m_manager_Detachment;
    Eigen::VectorXi m_manager_Type;
    Eigen::VectorXi m_manager_ID;

    Eigen::MatrixXd Default_Rate;

    Eigen::Matrix<short, Eigen::Dynamic, Eigen::Dynamic> ALMap;

    bool m_Debug{};

    void GetFromAPI(std::string &URL, Simulation &W, Model &M, Poco::LogStream &logstream);
};

#endif
