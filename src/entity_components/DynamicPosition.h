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

#ifndef SOLSTICE_DYNAMICPOSITION_H
#define SOLSTICE_DYNAMICPOSITION_H

#include "entity_component.h"


class DynamicPosition : public EntityComponent {

public:
    explicit DynamicPosition(std::size_t size) : EntityComponent("Dynamic Position") {
        m_entity_size = size;
        m_entity_X.setZero(size);
        m_entity_Y.setZero(size);
        m_entity_VX.setZero(size);
        m_entity_VY.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_X[i] = 0.0;
            m_entity_Y[i] = 0.0;
            m_entity_VX[i] = 1.0;
            m_entity_VY[i] = 1.0;
        }
    }

    DynamicPosition(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Dynamic Position") {
        std::string fragment = "DynamicPosition";
        Initialize(fragment, W, M, logstream);
    }

    void Resize(int NetworkSize, Poco::LogStream &logstream) {
        m_entity_size = NetworkSize;
        m_entity_X.resize(NetworkSize);
        m_entity_Y.resize(NetworkSize);
        m_entity_VX.resize(NetworkSize);
        m_entity_VY.resize(NetworkSize);
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
                m_entity_X[i] = object->getValue<double>("X");
                m_entity_Y[i] = object->getValue<double>("Y");
                m_entity_VX[i] = object->getValue<double>("VX");
                m_entity_VY[i] = object->getValue<double>("VY");
            }
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Unexpected structure for portfolio data")) << std::endl;
            abort();
        }

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 5 | Successfully Initialized Portfolio Size: " << file_size << std::endl;
        }

    }

    [[nodiscard]] auto GetX() const {
        return m_entity_X;
    }

    [[nodiscard]] double GetX(Eigen::Index i) const {
        return m_entity_X[i];
    }

    void SetX(Eigen::ArrayXd &X) {
        m_entity_X = X;
    }
    void SetX(double x, Eigen::Index i) {
        m_entity_X[i] = x;
    }

    [[nodiscard]] Eigen::ArrayXd GetY() const {
        return m_entity_Y;
    }
    void SetY(Eigen::ArrayXd X) {
        m_entity_Y = std::move(X);
    }

    void SetVX(Eigen::ArrayXd X) {
        m_entity_VX = std::move(X);
    }

    void SetVY(Eigen::ArrayXd X) {
        m_entity_VY = std::move(X);
    }

    [[nodiscard]] Eigen::ArrayXd GetVX() const {
        return m_entity_VX;
    }

    Eigen::ArrayXd GetVY() const {
        return m_entity_VY;
    }

    static std::string Serialize() {
        // create json object from
        // serialize json object
        return "SERIALIZED";
    }

    void Print(Poco::LogStream &logstream) {
        logstream.information() << m_component_Name << std::endl;
        logstream.information() << "----------------------------" << std::endl;
        for (int k = 0; k < m_entity_size; k++)
            logstream.information() << k << " " << m_entity_X[k] << std::endl;
    }

private:
    Eigen::ArrayXd m_entity_X;
    Eigen::ArrayXd m_entity_Y;
    Eigen::ArrayXd m_entity_VX;
    Eigen::ArrayXd m_entity_VY;
};

#endif //SOLSTICE_DYNAMICPOSITION_H
