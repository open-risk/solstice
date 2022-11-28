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
#include "AdjacencyMatrix.h"

void AdjacencyMatrix::AdjacencyMatrixFromJSON(const std::string &json, Poco::LogStream &logstream) {

    Poco::JSON::Parser jsonParser;

    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;
    Poco::JSON::Array::Ptr JsonArray;
    Poco::JSON::Array::Ptr JsonArray1;
    Poco::JSON::Array::Ptr JsonArray2;

    int m_size;

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 3 | Parsing adjacency matrix data from JSON." << std::endl;
    }

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        JsonArray = JsonObject->getArray("adjacency_matrix");
        // The first dimension (of a square adjacency matrix)
        m_size = JsonArray->size();
    }
    catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to parse adjacency matrix object. Check it is valid JSON"))
                            << std::endl;
        abort();
    }

    m_Graph.resize(m_size, m_size);

    // Read Adjacency Matrix
    try {
        Poco::JSON::Array::Ptr Row;
        for (int i = 0; i < m_size; i++) {
            Row = JsonArray->getArray(i);
            for (int j = 0; j < m_size; j++) {
                logstream.information() << i << "  " << j << std::endl;
                // Read boolean (0/1) values
                int conn_value = Row->get(j);
                m_Graph(i, j) = conn_value;
            }
        }
    }
    catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to resolve adjacency matrix values. Check it is valid JSON"))
                            << std::endl;
        abort();
    }

    // Output adjacency matrix

    if (LOG_LEVEL >= 1) {
        logstream.information() << ">> 5 | Adjacency Matrix: " << std::endl;
        if (LOG_LEVEL >= 3) {
            for (int i = 0; i < m_size; i++) {
                for (int j = 0; j < m_size; j++)
                    logstream.information() << m_Graph(i, j) << "\t";
                logstream.information() << std::endl;
            }
            logstream.information() << std::endl;
        }
    }
}

