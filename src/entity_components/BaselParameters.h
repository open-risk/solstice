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

#ifndef SOLSTICE_BaselParameters_H
#define SOLSTICE_BaselParameters_H

#include "entity_component.h"

/**
 * Basel Parameters Entity Component. Associates with a credit entity Basel Risk Parameters
 */
class BaselParameters : public EntityComponent {

public:
    explicit BaselParameters(int size) : EntityComponent("Basel Parameters") {
        m_entity_size = size;
        m_entity_Basel_Type.setZero(size);
        m_entity_Maturity.setZero(size);
        m_entity_EAD.setZero(size);
        m_entity_PD.setZero(size);
        m_entity_LGD.setZero(size);
        for (int i = 0; i < size; i++) {
            m_entity_Basel_Type[i] = 1;
            m_entity_Maturity[i] = 5;
            m_entity_EAD[i] = 1.0;
            m_entity_PD[i] = 0.1;
            m_entity_LGD[i] = 0.5;
        }
    }

    BaselParameters(Simulation &W, Model &M, Poco::LogStream &logstream) : EntityComponent("Basel Parameters") {
        std::string fragment = "BaselParameters";
        Initialize(fragment, W, M, logstream);
    }

    void Resize(int NetworkSize, Poco::LogStream &logstream) {
        m_entity_size = NetworkSize;
        m_entity_Basel_Type.resize(NetworkSize);
        m_entity_Maturity.resize(NetworkSize);
        m_entity_EAD.resize(NetworkSize);
        m_entity_PD.resize(NetworkSize);
        m_entity_LGD.resize(NetworkSize);
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
                m_entity_EAD[i] = object->getValue<double>("EAD");
                m_entity_LGD[i] = object->getValue<int>("LGD");
                m_entity_Maturity[i] = object->getValue<int>("Tenor");
                m_entity_PD[i] = object->getValue<double>("Rating");
                // Optional Parameters
                try {
                    m_entity_Basel_Type[i] = object->getValue<int>("Basel_Type");
                }
                catch (...) {
                    m_entity_Basel_Type[i] = 0;
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

    [[nodiscard]] Eigen::VectorXi GetEntityBaselType() const {
        return m_entity_Basel_Type;
    };

    [[nodiscard]] int Basel_Type(int Entity) const {
        return m_entity_Basel_Type[Entity];
    };

    [[nodiscard]] Eigen::VectorXi GetBaselType() const {
        return m_entity_Basel_Type;
    }

    [[nodiscard]] Eigen::VectorXd GetMaturity() const {
        return m_entity_Maturity;
    }

    [[nodiscard]] Eigen::VectorXd GetEAD() const {
        return m_entity_EAD;
    }

    [[nodiscard]] double GetTotalEAD() const {
        return m_entity_EAD.sum();
    }

    [[nodiscard]] Eigen::VectorXd GetPD() const {
        return m_entity_PD;
    }

    [[nodiscard]] Eigen::VectorXd GetLGD() const {
        return m_entity_LGD;
    }

    [[nodiscard]] double GetEntityEAD(int Entity) const {
        return m_entity_EAD[Entity];
    };

    [[nodiscard]] double GetEntityLGD(int Entity) const {
        return m_entity_LGD[Entity];
    };

//    [[nodiscard]] int MaturityMethod() const {
//        return m_MaturityMethod;
//    };


private:

    // Calculated variables
    Eigen::VectorXi m_entity_Basel_Type;
    Eigen::VectorXd m_entity_Maturity;
    Eigen::VectorXd m_entity_EAD;
    Eigen::VectorXd m_entity_PD;
    Eigen::VectorXd m_entity_LGD;  // Ex-ante (expected / modelled)
};

#endif //SOLSTICE_BaselParameters_H
