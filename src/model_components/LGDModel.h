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


#include "model_component.h"

class LGDModel : public ModelComponent {
public:
    LGDModel(const std::string &name, size_t periods) : ModelComponent(name) {
        m_component_Name = "LGD Model";
    };

    LGDModel(Simulation &W, Model &M, Poco::LogStream &logstream): ModelComponent("LGD Model")  {
        std::string fragment = "LGDModel";
        Initialize(fragment, W, M, logstream);
    }

    ~LGDModel() override = default;

    void Deserialize(std::string json, Poco::LogStream &logstream) {

        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;

        if (LOG_LEVEL >= 2) {
            logstream.information() << ">> 3 | Parsing factor LGD data from JSON." << std::endl;
        }

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
            JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        }
        catch (...) {
            logstream.information()
                    << BOLD(FRED("> ERROR: Failed to parse LGD Model object. Check it is valid JSON"))
                    << std::endl;
            abort();
        }

        try {
            m_LGDMethod = JsonObject->getValue<int>("LGDMethod");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading LGD Method")) << std::endl;
            abort();
        }
    }

    void SetLGDMethod(int x) {
        m_LGDMethod = x;
    };

    [[nodiscard]] int LGDMethod() const {
        return m_LGDMethod;
    }

    [[nodiscard]] double BetaA() const {
        return a;
    }

    [[nodiscard]] double BetaB() const {
        return b;
    }

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 2 | LGD Method:" << "\t" << m_LGDMethod << std::endl;
        logstream.information() << ">> 3 | Horizon: " << M.CalculationHorizon() << std::endl;

    }

private:

    int m_LGDMethod{};

    double a{};
    double b{};
};