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

#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "core/simulation.h"
#include "models/model.h"

/**
 * Base Class for Entity Components.
 */
class EntityComponent {
public:

    /**
     * 
     * @param name 
     */
    explicit EntityComponent(const std::string &name) {
        m_component_Name = name;
    }

    virtual ~EntityComponent() = default;

    /**
    * Parse a JSON object to obtain Entity Component Data.
    * JSON Array of fundamental types
     *
     * @param json
     * @param W
     * @param M
     * @param logstream
     */
    virtual void Deserialize(const std::string &json, Simulation &W, Model &M, Poco::LogStream &logstream);


    /**
     * JSON Array of fundamental types
     *
     * @param URL
     * @param W
     * @param M
     * @param logstream
     */
    void GetFromAPI(const std::string &URL, Simulation &W, Model &M, Poco::LogStream &logstream);

    /**
     * Read Entity Component Data from File
     *
     * @param filename
     * @param W
     * @param M
     * @param logstream
     */
    void ReadFromFile(const std::string &filename, Simulation &W, Model &M, Poco::LogStream &logstream);

    /**
     *
     * @param fragment
     * @param W
     * @param M
     * @param logstream
     */
    void Initialize(const std::string &fragment, Simulation &W, Model &M, Poco::LogStream &logstream);

    std::string GetName() {
        return m_component_Name;
    }

    size_t GetSize() {
        return m_entity_size;
    }

    void SetSize(size_t size) {
        m_entity_size = size;
    }

private:

protected:

    // Total entity (node) number
    size_t m_entity_size{};

    // The name of the component
    // Required for all components and must be distinct
    std::string m_component_Name;
    bool m_Debug{};

};


#endif // ENTITYCOMPONENT_H
