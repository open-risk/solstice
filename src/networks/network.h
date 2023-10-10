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

#ifndef NETWORK_H
#define NETWORK_H

#include "core/simulation.h"
#include "entity_types/GenericEntity.h"
#include "entity_relations/GenericRelation.h"
#include "models/model.h"

class Model;

class Network {
public:

    Network(Simulation &W, Model &M, Poco::LogStream &logstream);

    ~Network() = default;

    void Deserialize(const std::string &json, Simulation &W, Poco::LogStream &logstream) {
    };

    void ReadFromFile(const std::string &filename, Simulation &W, Poco::LogStream &logstream);

    void PrintEntities(int Level, Poco::LogStream &logstream);

    [[nodiscard]] bool SingleEntityFlag() const {
        return m_SingleEntityFlag;
    };

    [[nodiscard]] size_t GetEntityCount() const {
        size_t entity_count = 0;
        for (auto &pool: m_Entity_List)
            entity_count += pool->GetSize();
        return entity_count;
    };

    [[nodiscard]] size_t GetEdgeCount() const {
        return m_edge_size;
    };

    static int TotalEADEntities() {
        return 0;
    }

    [[nodiscard]] std::string GetNetworkName() const {
        return m_NetworkName;
    }

    [[nodiscard]] int PortfolioDataMode() const {
        return m_NetworkDataMode;
    };

    auto GetEntityList() {
        return &m_Entity_List;
    }

    auto GetRelationList() {
        return &m_Relation_List;
    }

    [[nodiscard]] int GetEntities() const {
        return m_Entities;
    };

    [[nodiscard]] int GetpEntities() const {
        return m_pEntities;
    };

    void SetEntities(int N) {
        m_Entities = N;
    };

    void Deserialize(const std::string &json, Poco::LogStream &logstream);

private:

    std::vector<std::shared_ptr<GenericEntity>> m_Entity_List;
    std::vector<std::shared_ptr<GenericRelation>> m_Relation_List;

    std::string m_NetworkName;

    int m_NetworkDataMode{};

    std::string network_data_modes[6] = {"Actual Local", "Actual API", "Synthetic Local", "Synthetic API",
                                         "From Configuration", "None"};

    bool m_SingleEntityFlag{};
    int m_edge_size{};
    int m_Entities{};
    int m_pEntities{};
    bool m_Debug{};

};

#endif // NETWORK_H
