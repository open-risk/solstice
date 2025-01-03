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

#include "networks/network.h"
#include "AdjacencyList.h"

void AdjacencyList::AdjacencyListFromJSON(const std::string &json, Poco::LogStream &logstream) {

    Poco::JSON::Parser jsonParser;

    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;
    Poco::JSON::Array::Ptr JsonArray;
    Poco::JSON::Array::Ptr JsonArray1;
    Poco::JSON::Array::Ptr JsonArray2;

    int m_edge_size;

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 3 | Parsing adjacency edge list data from JSON." << std::endl;
    }

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();

        JsonArray1 = JsonObject->getArray("from_node");
        JsonArray2 = JsonObject->getArray("to_node");
        m_From_Size = JsonObject->getValue<int>("node_count");
        m_To_Size = JsonObject->getValue<int>("node_count");
        m_edge_size = JsonArray1->size();
    }
    catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to parse adjacency edge objects. Check it is valid JSON"))
                            << std::endl;
        abort();
    }

    m_from_Node.resize(m_From_Size);
    m_to_Node.resize(m_To_Size);

    // Read Adjacency Lists
    try {
        for (int e = 0; e < m_edge_size; e++) {
            m_from_Node(e) = JsonArray1->get(e);
            m_to_Node(e) = JsonArray2->get(e);
        }
    }
    catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Failed to resolve adjacency edge values. Check it is valid JSON"))
                            << std::endl;
        abort();
    }

    // Output network edges

    if (LOG_LEVEL >= 1) {
        logstream.information() << ">> 5 | Adjacency Lists: " << std::endl;
        if (LOG_LEVEL >= 3) {
            for (int e = 0; e < m_edge_size; e++) {
                logstream.information() << "From : " << m_from_Node(e) << " To " << m_to_Node(e) << std::endl;
            }
        }
    }
}

