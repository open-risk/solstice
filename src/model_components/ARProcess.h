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

#include "model_component.h"

/**
 * Autoregressive Process Model Component.
 * For the simulation of AR(1) processes
 */
class ARProcess : public ModelComponent {
public:

    ARProcess(std::string &name, size_t order) : ModelComponent(name) {
        m_component_Name = "AR Process";
        mu = 0.0;
        x.setZero(order + 1);
        phi.setZero(order + 1);
    };

    ARProcess(Simulation &W, Model &M, Poco::LogStream &logstream) : ModelComponent("AR Process") {
        std::string fragment = "ARProcess";
        Initialize(fragment, W, M, logstream);
    }

    ~ARProcess() override = default;

    void Deserialize(const std::string &json, Poco::LogStream &logstream) {

        Poco::JSON::Parser jsonParser;
        Poco::Dynamic::Var ParsedJsonResult;
        Poco::JSON::Object::Ptr JsonObject;

        try {
            jsonParser.parse(json);
            ParsedJsonResult = jsonParser.result();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem parsing component data")) << std::endl;
            abort();
        }

        try {
            JsonObject = ParsedJsonResult.extract<Poco::JSON::Object::Ptr>();
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to extract component data. Check it is valid JSON"))
                              << std::endl;
            abort();
        }

        Poco::JSON::Object::Ptr AR;

        try {
            AR = JsonObject->getObject("AR Model Data");
        }
        catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Failed to parse AR data. Check it is valid JSON")) << std::endl;
            abort();
        }
        try {
            order = JsonObject->getValue<int>("Order");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading model order")) << std::endl;
            abort();
        }

        try {
            mu = AR->getValue<double>("Offset");
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading model offset")) << std::endl;
            abort();
        }

        Poco::JSON::Array::Ptr PhiArr;
        try {
            PhiArr = AR->getArray("Phi");
            for (int j = 0; j < order; j++) {
                double val = PhiArr->get(j);
                phi[j] = val;
            }
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading phi values")) << std::endl;
            abort();
        }

        Poco::JSON::Array::Ptr XArr;
        try {
            XArr = AR->getArray("Initial Conditions");
            for (int j = 0; j < order + 1; j++) {
                double val = XArr->get(j);
                x[j] = val;
            }
        } catch (...) {
            logstream.error() << BOLD(FRED("> ERROR: Problem reading x values")) << std::endl;
            abort();
        }
    }

    [[nodiscard]] double Mu() const {
        return mu;
    }

    [[nodiscard]] Eigen::VectorXd Phi() const {
        return phi;
    }

    [[nodiscard]] Eigen::VectorXd X() const {
        return x;
    }

    [[nodiscard]] int Order() const {
        return order;
    }

    void PrintModelData(int Level, Model &M, Poco::LogStream &logstream) const {

        logstream.information() << ">> 3 | -------- Model Data --------" << std::endl;
        logstream.information() << ">> 2 | Process Order:" << "\t" << order << std::endl;
    }

private:
    // AR Model Order
    int order{};

    double mu{}; // offset (usually 0)
    Eigen::VectorXd phi; // autoregression coefficient (1 -> Random Walk, 0->
    Eigen::VectorXd x; // initial conditions of the AR process
};