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

#ifndef SOLSTICE_BusinessModel_H
#define SOLSTICE_BusinessModel_H

#include "entity_component.h"

/**
 * Business Model Entity Component. Characterises an entity as a business and provides
 * related information.
 */
class BusinessModel : public EntityComponent {

public:
    explicit BusinessModel(int size) : EntityComponent("Business Model") {
        m_entity_size = size;
        m_entity_Country.setZero(size);
        m_entity_Sector.setZero(size);
        m_entity_Annual_Sales.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_Country[i] = 1;
            m_entity_Sector[i] = 1;
            m_entity_Annual_Sales[i] = 100.0;
        }
    }

    BusinessModel(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Business Model") {
        std::string fragment = "BusinessModel";
        Initialize(fragment, W, M, logstream);
    }

    void Resize(int NetworkSize, Poco::LogStream &logstream) {
        m_entity_size = NetworkSize;
        m_entity_Country.resize(NetworkSize);
        m_entity_Sector.resize(NetworkSize);
        m_entity_Annual_Sales.resize(NetworkSize);
    };

    void Deserialize(const std::string &json,  Simulation &W, Model &M, Poco::LogStream &logstream) override {

        Poco::JSON::Array::Ptr arr;
        Poco::JSON::Object::Ptr object;
        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
            arr = ParsedJsonResult.extract<Poco::JSON::Array::Ptr>();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem parsing entity data")) << std::endl;
            abort();
        };

        // The size of the data set
        int file_size = arr->size();
        if (LOG_LEVEL >= 1) {
            logstream.information() << ">> 5 | Entity File Size: " << file_size << std::endl;
        }
        m_entity_size = file_size;

        Resize(file_size, logstream);

        try {
            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 5 | Reading Entity Data: " << file_size << std::endl;
            }
            for (int i = 0; i < file_size; i++) {
                object = arr->getObject(i);
                m_entity_Country[i] = object->getValue<int>("Country");
                m_entity_Sector[i] = object->getValue<int>("Sector");
                // Optional Parameters
                try {
                    m_entity_Annual_Sales[i] = object->getValue<double>("Annual_Sales");
                }
                catch (...) {
                    m_entity_Annual_Sales[i] = 1.0;
                }
            }
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Unexpected structure for portfolio data")) << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 5 | Successfully Initialized Network Size: " << file_size << std::endl;
        }

    }

    [[nodiscard]] int Sector(int Entity) const {
        return m_entity_Sector[Entity];
    };

    Eigen::VectorXi GetSector() {
        return m_entity_Sector;
    }

    [[nodiscard]] double Annual_Sales(int Entity) const {
        return m_entity_Annual_Sales[Entity];
    };
    [[nodiscard]] Eigen::VectorXi GetEntityCountry() const {
        return m_entity_Country;
    };
    [[nodiscard]] int Country(int Entity) const {
        return m_entity_Country[Entity];
    };

private:
    // Geolocation information (Country of Operation)
    Eigen::VectorXi m_entity_Country;
    // Business Model Information
    Eigen::VectorXi m_entity_Sector;
    // Financials
    Eigen::VectorXd m_entity_Annual_Sales;
};

#endif //SOLSTICE_BusinessModel_H
