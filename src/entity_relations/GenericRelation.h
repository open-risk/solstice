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

#ifndef GENERICRELATION_H
#define GENERICRELATION_H

#include <any>
#include "core/simulation.h"

class GenericRelation {
public:

    explicit GenericRelation(const std::string &name, std::size_t from_size, std::size_t to_size) {
        m_Relation_Name = name;
        m_From_Size = from_size;
        m_To_Size = to_size;
    };

    virtual ~GenericRelation() = default;

    std::string GetName() {
        return m_Relation_Name;
    }

    [[nodiscard]] std::size_t GetFromSize() const {
        return m_From_Size;
    }

    [[nodiscard]] std::size_t GetToSize() const {
        return m_To_Size;
    }

    [[nodiscard]] int AdjacencyDataMode() const {
        return m_AdjacencyDataMode;
    };

private:

    std::string m_Relation_Name;

    int m_AdjacencyDataMode{};

protected:
    std::size_t m_From_Size;
    std::size_t m_To_Size;

    bool m_Debug{};
};

#endif
