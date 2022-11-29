/*################################################################################
  ##
  ##   Copyright (C) 2020-2022 Open Risk (www.openriskmanagement.com)
  ##
  ##   This file is part of the tailRisk C++ library.
  ##
  ##   Licensed under the Apache License, Version 2.0 (the "License");
  ##   you may not use this file except in compliance with the License.
  ##   You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ################################################################################*/


/**
 * File:   random_var.h
 * Date: Mon Nov  9 10:31:35 CET 2020
 */

#ifndef RANDOM_VARIABLE_H
#define RANDOM_VARIABLE_H

#include <iostream>
#include <Eigen/Dense>

class RandomVar {
    friend std::ostream &operator<<(std::ostream &os, const RandomVar &R);

public:

    // constructor with size and empirical distribution type
    // Type 0 -> histogram
    // Type 1 -> samples

    RandomVar(size_t S, int type) {
        if (type == 0) {
            m_type = 0;
            m_P.resize(S);
            m_C.resize(S);
            m_X.resize(S);
            m_size = S;
        } else if (type == 1) {
            m_type = 1;
            m_S.resize(S);
            m_size = S;
        } else {
            std::cout << "Error in random variable representation type" << std::endl;
        }
    }

    RandomVar(Eigen::ArrayXd x, Eigen::ArrayXd p, const int size) {
        m_type = 0;
        m_size = size;
        m_X.resize(size);
        m_P.resize(size);
        m_C.resize(size);
        for (int i = 0; i < size; i++) {
            m_X[i] = x[i];
            m_P[i] = p[i];
        }
    }

    // overload assignment operator
    RandomVar &operator=(const RandomVar &R);

    [[nodiscard]] size_t size() const {
        return m_size;
    };

    [[nodiscard]] int type() const {
        return m_type;
    };

    [[nodiscard]] double getP(int index) const {
        return m_P[index];
    };

    [[nodiscard]] double getC(int index) const {
        return m_C[index];
    };

    [[nodiscard]] double getX(int index) const {
        return m_X[index];
    };

    [[nodiscard]] double getS(int index) const {
        return m_S[index];
    };

    [[nodiscard]] double Average() const;

    [[nodiscard]] double Mean() const;

    [[nodiscard]] double Median() const;

    [[nodiscard]] double Variance() const;

    [[nodiscard]] double Vol() const;

    [[nodiscard]] double StandardDeviation() const;

    [[nodiscard]] double Kurtosis() const;

    [[nodiscard]] double Skeweness() const;

    [[nodiscard]] double ExpectedShortFall(double alpha) const;

    [[nodiscard]] double ExceedanceProbability(int index) const;

    [[nodiscard]] double MeanExcess(int index) const;

    [[nodiscard]] double Quantile(double alpha) const;

    [[nodiscard]] double VaR(double alpha) const;

    [[nodiscard]] int Quantile_Index(double alpha) const;

    void setP(int index, double arg) {
        m_P[index] = arg;
    };

    void setC(int index, double arg) {
        m_C[index] = arg;
    };

    void setX(int index, double arg) {
        m_X[index] = arg;
    };

    void setS(int index, double arg) {
        m_S[index] = arg;
    };

    void Sort();

    void Cumulative();

    void Probability();

    void ReadFromJSON(const std::string &filename);

    void Print();

private:
    // 0 Type: exact representation
    // 1 Type: sampling representation
    int m_type;
    int m_size;
    Eigen::ArrayXd m_P{}; // storage of probability mass
    Eigen::ArrayXd m_C{}; // storage of cumulative probability
    Eigen::ArrayXd m_X{}; // storage of discrete values (random variable range)
    Eigen::ArrayXd m_S{}; // storage of sampling data from simulation experiments
};

#endif