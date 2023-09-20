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

#ifndef GENERICENTITY_H
#define GENERICENTITY_H

#include "core/simulation.h"

/// \brief Generic Entity Pool Class (Base Class)

class GenericEntity {
public:

    explicit GenericEntity(const std::string& name, std::size_t size) {
        m_Pool_Name = name;
        m_Pool_Size = size;
    };

    explicit GenericEntity(const std::string& name) {
        m_Pool_Name = name;
    };

    virtual ~GenericEntity() = default;

    virtual void Print(Poco::LogStream &logstream){};

    std::string GetName(){
        return m_Pool_Name;
    }

    void SetName(std::string name){
        m_Pool_Name = name;
    }

    void SetSize(size_t size) {
        m_Pool_Size = size;
    }

    [[nodiscard]] std::size_t GetSize() const{
        return m_Pool_Size;
    }

    void AddComponent(const std::string& component){
        m_Component_List.push_back(component);
    }

    std::size_t GetComponentCount(){
        return m_Component_List.size();
    }

    auto GetComponents(){
        return m_Component_List;
    }

private:

protected:
    // The name of the entity portfolio (for reference purposes)
    std::string m_Pool_Name;

    // Total entity (node) number
    std::size_t m_Pool_Size{};

    std::vector<std::string> m_Component_List;
    bool m_Debug = false;
};


#endif
