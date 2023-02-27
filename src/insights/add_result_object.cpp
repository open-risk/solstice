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

#include "insights.h"
#include "utils/random_var.h"
#include "RandomVar1D.h"

int ACCURATE_DIGITS = 5;

Eigen::MatrixXd Slice(Eigen::Tensor<double, 3> T, Eigen::Index k) {
    Eigen::MatrixXd result;
    for (Eigen::Index i = 0; i < T.dimension(1); i++) {
        for (Eigen::Index j = 0; j < T.dimension(2); j++) {
            result(i, j) = T(i, j, k);
        }
    }
    return result;
}

void
Insights::add_result_object(int choice, Poco::JSON::Array &results_data, Simulation &W, Network &P, Model &M,
                            Model_Data &MD, Managers &MA, Scenarios &S, Poco::LogStream &logstream) {

    std::string Message;
    Poco::JSON::Object result_object;
    Poco::JSON::Object histogram1D;
    RandomVar RV(M.MacroScenarios(), 1);

    switch (choice) {

        default:
        case 33:

            if (M.ModelFamilyID() != 14) {
                Message = "RandomVar1D not available in this Model Family";
                reporting_message(LOG_LEVEL, Message, logstream);
                result_object.set("Message", Message);
                result_object.set("ID", choice);
                results_data.add(result_object);
                break;
            }

            for (auto &rc: m_Result_Components) {
                logstream.information() << rc->GetName() << std::endl;
                if (rc->GetName() == "Random Var 1D") {
                    auto *ER = dynamic_cast<RandomVar1D *>(rc.get());
                    RV = ER->GetRV();
                }
            }
            RandomVar H = RV.Histogram(2);

            histogram1D.clear();
            Poco::JSON::Array x_array;
            Poco::JSON::Array y_array;

            for (int i = 0; i < H.size() - 1; i++) {
                x_array.add(H.getX(i));
                y_array.add(H.getP(i));
            }
            histogram1D.set("x", x_array);
            histogram1D.set("y", y_array);

            result_object.set("Data", histogram1D);
            result_object.set("Type", "1D Histogram");
            result_object.set("Message", "Entity Position Distribution");
            result_object.set("ID", choice);
            results_data.add(result_object);
            break;
    }
};

