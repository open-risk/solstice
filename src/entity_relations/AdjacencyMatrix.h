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

#include "networks/network.h"

class AdjacencyMatrix : public GenericRelation {
public:
    explicit AdjacencyMatrix(size_t edges, Poco::LogStream &logstream)
            : GenericRelation("Adjacency Matrix", edges, edges) {
        m_Graph.setZero(edges, edges);
        int initialization_mode;
        std::string json;
        if (initialization_mode) {
            AdjacencyMatrixFromJSON(json, logstream);
        }
    };

    [[nodiscard]] Eigen::MatrixXi GetG() {
        return m_Graph;
    }

    void AdjacencyMatrixFromJSON(const std::string &json, Poco::LogStream &logstream);

private:
    // Network adjacency matrix (graph) between network nodes
    Eigen::MatrixXi m_Graph;
};