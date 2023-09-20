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

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "core/simulation.h"
#include "models/model.h"
#include "model_component.h"

class ModelComponent;

/**
 * Model Data container of Model Components.
 */
class Model_Data {
public:

    Model_Data(Simulation &W, Model &M, Poco::LogStream &logstream);

    ~Model_Data() = default;

    void Print(Poco::LogStream &logstream);

    static void PrintModelData(int Level, Model &M, Poco::LogStream &logstream);

    auto GetModelComponent() {
        return &m_Model_Components;
    }

private:

    std::vector<std::shared_ptr<ModelComponent>> m_Model_Components{};

    bool m_Debug = true;

};

#endif // MODEL_DATA_H
