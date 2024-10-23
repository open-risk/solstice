/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
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

#include "managers.h"

void Managers::Deserialize(const std::string& json, Simulation &W, Poco::LogStream &logstream) {

    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var ParsedJsonResult;
    Poco::JSON::Object::Ptr JsonObject;

    if (LOG_LEVEL >= 3) {
        logstream.trace() << json << std::endl;
    }

    try {
        jsonParser.parse(json);
        ParsedJsonResult = jsonParser.result();
        JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem parsing manager data")) << std::endl;
        abort();
    };

    try {
        m_LiabilityMethod = JsonObject->getValue<int>("LiabilityMethod");
    } catch (...) {
        logstream.error() << BOLD(FRED("> ERROR: Problem reading manager method")) << std::endl;
        abort();
    }

    Poco::JSON::Array::Ptr JsonArray;
    Poco::JSON::Object::Ptr Tranche;

    if (m_LiabilityMethod == 1) {
        try {

            m_Initial_Equity_Capital = JsonObject->getValue<double>("EquityCapital");
            if (LOG_LEVEL >= 1) {
                logstream.information() << ">> 6 | Read Equity Capital: " << m_Initial_Equity_Capital << std::endl;
            }
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading Liability Data (Method 1)")) << std::endl;
            abort();
        };
    } else if (m_LiabilityMethod == 2) {

        try {

            JsonArray = JsonObject->getArray("LiabilityStructure");
            if (LOG_LEVEL >= 1) {
                logstream.information() << ">> 6 | Read Liability Structure" << std::endl;
            }

            m_size = JsonArray->size();
            m_manager_Attachment.setZero(m_size);
            m_manager_Detachment.setZero(m_size);
            m_manager_Tenor.setZero(m_size);
            m_manager_Type.setZero(m_size);
            m_manager_ID.setZero(m_size);

            for (int i = 0; i < m_size; i++) {
                Tranche = JsonArray->getObject(i);
                m_manager_Attachment[i] = Tranche->getValue<double>("Attachment");
                m_manager_Detachment[i] = Tranche->getValue<double>("Attachment");
                m_manager_ID[i] = Tranche->getValue<int>("Liability_ID");
                m_manager_Type[i] = Tranche->getValue<int>("Type");
                m_manager_Tenor[i] = Tranche->getValue<int>("Tenor");
            }

        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading Liability Structure (Method 2)")) << std::endl;
            abort();
        };

        try {

            JsonArray = JsonObject->getArray("ALMap");
            if (LOG_LEVEL >= 1) {
                logstream.information() << ">> 6 | Read ALMap" << std::endl;
            }

            int map_size = JsonArray->size();
            if (map_size != m_size) {
                logstream.information() << ">> 6 | ALMap size differs from Liability Count" << std::endl;
                abort();
            }

            m_manager_Attachment.setZero(m_size);
            m_manager_Detachment.setZero(m_size);
            m_manager_Tenor.setZero(m_size);
            m_manager_Type.setZero(m_size);
            m_manager_ID.setZero(m_size);

            for (int i = 0; i < m_size; i++) {
                Tranche = JsonArray->getObject(i);
                m_manager_Attachment[i] = Tranche->getValue<double>("Attachment");
                m_manager_Detachment[i] = Tranche->getValue<double>("Attachment");
                m_manager_ID[i] = Tranche->getValue<int>("Liability_ID");
                m_manager_Type[i] = Tranche->getValue<int>("Type");
                m_manager_Tenor[i] = Tranche->getValue<int>("Tenor");
            }

        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading Liability Structure (Method 2)")) << std::endl;
            abort();
        };

    }

}