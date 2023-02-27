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

#include "networks/network.h"

class AdjacencyList : public GenericRelation {
public:
    explicit AdjacencyList(std::size_t from, std::size_t to, Poco::LogStream &logstream)
            : GenericRelation("Adjacency List", from, to) {
        m_from_Node.setZero(from, to);
        m_to_Node.setZero(from, to);
        int initialization_mode;
        std::string json;
        if (initialization_mode) {
            AdjacencyListFromJSON(json, logstream);
        }
    };

    [[nodiscard]] Eigen::VectorXi GetFromNode() {
        return m_from_Node;
    }

    [[nodiscard]] Eigen::VectorXi GetToNode() {
        return m_to_Node;
    }

    void AdjacencyListFromJSON(const std::string &json, Poco::LogStream &logstream);

private:

    // Network edge information (two lists of from / to nodes)
    Eigen::VectorXi m_from_Node;
    Eigen::VectorXi m_to_Node;

};