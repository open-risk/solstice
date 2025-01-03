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

#include "models/model.h"

void
Model::compute_simulated_multifactor_scenarios(int fs, Eigen::MatrixXd &L, Eigen::Tensor<double, 3> &z3, int CorrelationMethod,
                                               int Factors, Poco::LogStream &logstream) {

    int TimePoints = this->CalculationHorizon() + 1;
    int BranchScenarios = this->BranchScenarios();
    int RiskHorizon = this->GetRiskHorizon();

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 8 | Correlation Method: " << CorrelationMethod << std::endl;
        logstream.information() << ">> 8 | Macro Scenarios: " << this->MacroSimulations() << std::endl;
        logstream.information() << ">> 8 | Branch Scenarios: " << this->BranchScenarios() << std::endl;
        logstream.information() << ">> 8 | Time Points: " << this->TimePoints() << std::endl;
        logstream.information() << ">> 8 | Temporal Interval: " << this->Interval() << std::endl;
    }

    double dt_root = 1.0;
    if (m_Interval == 1) {
        dt_root = sqrt(1.0 / 12.0);
    } else {
        dt_root = 1.0;
    }

    // TODO
    Eigen::Tensor<double, 3> Z_Shock; // = randn(MacroScenarios, TimePoints, m_Factors);
    Eigen::Tensor<double, 3> F_Shock; // = zeros(MacroScenarios, TimePoints, m_Factors);

    if (CorrelationMethod == 0) {


        for (int es = 0; es < BranchScenarios; es++) {
            int ms = fs * BranchScenarios + es;
            for (int k = 1; k < TimePoints; k++) {
                for (int f = 0; f < Factors; f++) {
                    double Z_Shock = 0; // TODO randn() * dt_root;
                    if (es == 0) { // Root Scenario
                        z3(ms, k, f) = z3(ms, k - 1, f) + Z_Shock;
                    } else if (es > 0 && k <= RiskHorizon) { // Branch scenarios (common first points with Root)
                        z3(ms, k, f) = z3(ms - 1, k, f);
                    } else if (es > 0 && k > RiskHorizon) {
                        z3(ms, k, f) = z3(ms, k - 1, f) + Z_Shock;
                    }
                }
            }
        }
    } else {

//        Eigen::MatrixXd L = Model_Data.GetL();

        for (int k = 1; k < TimePoints; k++) {
            for (int f1 = 0; f1 < Factors; f1++) {
                for (int f2 = 0; f2 < Factors; f2++) {
                    F_Shock(fs, k, f1) += L(f1, f2) * Z_Shock(fs, k, f2) * dt_root;
                }
            }
        }
    }

    for (int k = 1; k < TimePoints; k++) {
        for (int f = 0; f < Factors; f++) {
            z3(fs, k, f) = z3(fs, k - 1, f) + F_Shock(fs, k, f);
        }
    }
}

