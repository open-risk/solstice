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

#include "models/model.h"
#include "model_component.h"

class MarkovChain : public ModelComponent {
public:

    explicit MarkovChain(const std::string &name) : ModelComponent(name) {
        m_component_Name = "Markov Chain";
        T.setZero();
    };

    MarkovChain(const std::string &name, Eigen::Index states, Eigen::Index periods) : ModelComponent(name) {
        m_States = states;
        T.resize(states, states);
    };

    MarkovChain(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("Markov Chain"){;
        std::string fragment = "MarkovChain";
        Initialize(fragment, W, M, logstream);
    }

    ~MarkovChain() override = default;

    void Deserialize(std::string json, Poco::LogStream &logstream) {
    }

    [[nodiscard]] int States() const {
        return m_States;
    };

    Eigen::MatrixXd GetT() {
        return T;
    }

private:
    Eigen::Index m_States{}; // the total number of states in the chain
    Eigen::MatrixXd T; // Transition Matrix
};