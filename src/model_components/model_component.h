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

#ifndef ModelCOMPONENT_H
#define ModelCOMPONENT_H

#include <filesystem>
#include "core/simulation.h"
#include "models/model.h"

class ModelComponent {
public:

    explicit ModelComponent(const std::string &name) {
        m_component_Name = name;
    }

    virtual ~ModelComponent() = default;

    virtual void Deserialize(const std::string &json, Simulation &W, Model &M, Poco::LogStream &logstream);

    void ReadFromFile(const std::string &filename, Simulation &W, Model &M, Poco::LogStream &logstream);

    void GetFromAPI(const std::string &URL, Simulation &W, Model &M, Poco::LogStream &logstream);

    void Initialize(const std::string& fragment, Simulation &W, Model &M, Poco::LogStream &logstream);

    std::string GetName() {
        return m_component_Name;
    }

private:

    int m_entity_size{};

protected:

    std::string m_component_Name;
    bool m_Debug{};
};

#endif
