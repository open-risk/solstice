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

#ifndef ResultCOMPONENT_H
#define ResultCOMPONENT_H

#include "core/simulation.h"


class ResultComponent {
public:

    explicit ResultComponent(const std::string &name) {
        m_component_Name = name;
    }

    virtual ~ResultComponent() = default;

    std::string GetName() {
        return m_component_Name;
    }

private:

    // Total Result (node) number
    int m_Result_size{};

    bool m_Debug{};

protected:

    // The name of the component
    // Required for all components and must be distinct
    std::string m_component_Name;

};


#endif
