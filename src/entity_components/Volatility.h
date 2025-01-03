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

#ifndef SOLSTICE_Volatility_H
#define SOLSTICE_Volatility_H

#include "entity_component.h"

class Volatility : public EntityComponent {

public:
    explicit Volatility(int size) : EntityComponent("Volatility") {
        m_entity_size = size;
        m_sigma.setZero(size);
        for (int i = 0; i < size; i++) {
            m_sigma[i] = 1.0;
        }
    }

    Volatility(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Volatility") {
        std::string fragment = "Volatility";
        Initialize(fragment, W, M, logstream);
    }

    void Resize(int NetworkSize, Poco::LogStream &logstream) {
        m_entity_size = NetworkSize;
        m_sigma.resize(NetworkSize);
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

        Resize(file_size, logstream);

        try {
            if (LOG_LEVEL >= 2) {
                logstream.information() << ">> 5 | Reading Entity Data: " << file_size << std::endl;
            }
            for (int i = 0; i < file_size; i++) {
                object = arr->getObject(i);
                m_sigma[i] = object->getValue<double>("sigma");
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

    [[nodiscard]] Eigen::ArrayXd GetSigma() const {
        return m_sigma;
    }
    [[nodiscard]] double Sigma(int Entity) const {
        return m_sigma[Entity];
    };

    void Print(Poco::LogStream &logstream) {
        logstream.information() << m_component_Name << std::endl;
        logstream.information() << "----------------------------" << std::endl;
        for (int i = 0; i < m_entity_size; i++)
            logstream.information() << i << " " << m_sigma[i] << std::endl;
    }

private:
    // volatility metric
    Eigen::ArrayXd m_sigma;
};


#endif //SOLSTICE_Volatility_H
