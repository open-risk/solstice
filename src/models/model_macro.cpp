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

#include "stats.hpp"

#include "model.h"
#include "model_components/CorrelationMatrix.h"

#include "insights/RandomVar1D.h"

void Model::model_macro(Network &Po,
                        Scenarios &S,
                        Model_Data &MD,
                        Managers &MA,
                        Insights &MR,
                        Poco::LogStream &logstream) {

    logstream.information() << ">> 8 | Starting Macro Calculation" << std::endl;

    stats::rand_engine_t engine(123);

    // Get Required Model Components
    int CorrelationMethod;
    double rho;
    Eigen::MatrixXd Rho;
    Eigen::VectorXd beta;
    double mu;
    Eigen::VectorXd Phi;
    Eigen::MatrixXd L;

    // Correlation Matrix for Simulated Scenarios (Methods 1,3,4)
    for (auto &mc: *MD.GetModelComponent()) {
        if (mc->GetName() == "Correlation Matrix") {
            auto *CM = dynamic_cast<CorrelationMatrix *>(mc.get());
            rho = CM->GetCorr() * CM->GetCorr();
            Rho = CM->GetRho();
            CorrelationMethod = CM->CorrelationMethod();
            L = CM->GetL();
        }
    }

    int Horizon = this->CalculationHorizon();
    int TimePoints = Horizon + 1;

    int MacroScenarios;
    if (this->ScenarioMethod()== 1 || this->ScenarioMethod() == 3 || this->ScenarioMethod() == 4) {
        MacroScenarios = m_MacroScenarios;
    }
    else if (this->ScenarioMethod() == 2) {
        MacroScenarios = S.GetFactorScenarios();
    }

    Eigen::MatrixXd F; // Scenario values
    Eigen::VectorXd Pr; // Scenario probabilities
    int Factors;
    if (this->ScenarioMethod() == 2 || this->ScenarioMethod() == 3 || this->ScenarioMethod() == 4){
        F = S.GetFactorData();
        Pr = S.GetProbabilities();
        Factors = S.GetFactors();
    }


    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | Initialized parameters" << std::endl;
    }

    Eigen::Tensor<double, 3> Macro_Scenarios;
    Eigen::Tensor<double, 3> z3;
    z3.resize(MacroScenarios, TimePoints, Factors);

    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | Memory Initialized" << std::endl;
    }

    /**
     *
     *    SYSTEM INVOCATION
     *
     */

    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | Starting Macro Scenario Simulation" << std::endl;
    }

    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | Scenario Method: " << this->ScenarioMethod() << std::endl;
    }

    if (this->ScenarioMethod() == 0) {
        logstream.warning() << ">> 8 | ERROR: Scenario Method not compatible with Model Family" << std::endl;
        abort();
    }

    /**
    *  Scenario Method 1: Calculate Macro factors in Simulated scenario mode
    *
    */
    if (this->ScenarioMethod() == 1) {

        if (LOG_LEVEL >= 1) {
            logstream.warning() << ">> 8 | Simulating Macro Scenarios" << std::endl;
        }
        for (int fs = 0; fs < MacroScenarios; fs++) {
            compute_simulated_multifactor_scenarios(fs, L, z3, CorrelationMethod, Factors, logstream);
        }
        if (LOG_LEVEL >= 1) {
            logstream.warning() << ">> 8 | Calculated Simulated Macro Scenario" << std::endl;
        }

    } else {
        logstream.warning() << ">> 8 | Invalid Scenario or Model Family" << std::endl;
        abort();
    }

    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | " << this->MacroScenarios() << "  Macro Scenarios Generated " << std::endl;
    }

    Macro_Scenarios = S.GetZ3();

    if (LOG_LEVEL >= 1) {
        logstream.warning() << ">> 8 | Macro Scenarios Generated " << std::endl;
        logstream.warning() << ">> 8 | Moments of Systematic Factor: " << std::endl;
        if (LOG_LEVEL >= 3) {
            logstream.warning() << ">> 8 | Scenario Values " << std::endl;
            logstream.warning() << Macro_Scenarios;
        }
    }

}

