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
 * File: random_var.cpp
 * Date: Mon Nov  9 10:31:35 CET 2020
 */

#include <cmath>
#include <iostream>
#include <cassert>
#include <fstream>


#include <Poco/JSON/Parser.h>
#include "random_var.h"

using namespace Poco;

// assignement operator
RandomVar &RandomVar::operator=(const RandomVar &R) {
    assert(R.size() == this->size()); // check that size matches
    for (size_t i = 0; i < R.size(); i++) {
        this->setX(i, R.getX(i));
        this->setP(i, R.getP(i));
        this->setC(i, R.getC(i));
    }
    return (*this);
};

/**
 * Sort the sampling data
 */
void RandomVar::Sort() {
    std::sort(m_S.begin(), m_S.end());
}

/**
 * Compute the cumulative distribution function
 */
void RandomVar::Cumulative() {
    m_C[0] = m_P[0]; // starting value
    for (size_t i = 1; i < m_P.size(); i++)
        m_C[i] = m_C[i - 1] + m_P[i];  // add probability mass
}

/**
 * Compute the probability density
 */
void RandomVar::Probability() {
    m_P[0] = m_C[0];  // starting valued
    for (size_t i = 1; i < m_P.size(); i++)
        m_P[i] = m_C[i] - m_C[i - 1];  // incremental probability mass
}

/**
 * Compute the average value
 * The calculation depends on how the data are stored
 */
double RandomVar::Average() const {
    double expectation = 0.0;
    // from probability mass
    if (m_type == 0) {
        for (size_t i = 0; i < m_P.size(); i++) {
            expectation += m_P[i] * m_X[i];
        }
    } // from samples
    else if (m_type == 1) {
        for (size_t i = 0; i < m_S.size(); ++i) {
            expectation += m_S[i];
        }
        expectation /= m_S.size();
    }
    return expectation;
}

/**
 * Synonym for Average
 */
double RandomVar::Mean() const {
    return Average();
}

/**
 * Median as Quantile(0.5)
 */
double RandomVar::Median() const {
    return Quantile(0.5);
}

/**
 * Compute the variance
 * The calculation depends on how the data are stored
 */
double RandomVar::Variance() const {
    double var = 0;
    if (m_type == 0) {
        for (size_t i = 0; i < m_P.size(); i++)
            var += m_P[i] * m_X[i] * m_X[i];
        var -= Average() * Average();
    } else if (m_type == 1) {
        for (size_t i = 0; i < m_S.size(); i++)
            var += m_S[i] * m_S[i];
        var /= m_S.size();
        var -= Average() * Average();
    }
    return var;
}

/**
 * Compute volatility (standard deviation)
 */
double RandomVar::Vol() const {
    return sqrt(Variance());
}
double RandomVar::StandardDeviation() const {
    return Vol();
}

/**
 * Compute the skeweness
 * The calculation depends on how the data are stored
 */
double RandomVar::Skeweness() const {
    double skew = 0;
    if (m_type == 0) {
        double mean = Average();
        for (size_t i = 0; i < m_P.size(); i++)
            skew += m_P[i] * pow(m_X[i] - mean, 3);
        skew = skew / pow(Variance(), 3 / 2);
    }
    return skew;
}

/**
 * Compute the kurtosis
 * The calculation depends on how the data are stored
 */
double RandomVar::Kurtosis() const {
    double kurt = 0;
    if (m_type == 0){
        double mean = Average();
        for (size_t i = 0; i < m_P.size(); i++)
            kurt += m_P[i] * pow(m_X[i] - mean, 4);
        kurt = kurt / pow(Variance(), 2);
    }
    return kurt;
}

/**
 * Compute the Quantile Index
 * The calculation depends on how the data are stored
 */
int RandomVar::Quantile_Index(double alpha) const {
    int index = 0;
    if (m_type == 0)
    {
        for (int i = 0; i < m_P.size(); i++) {
            if (m_C[i] > 1 - alpha) {
                index = i;
                break;
            }
        }
    }
    return index;
}

/**
 * Compute the Quantile
 * The calculation depends on how the data are stored
 */
double RandomVar::Quantile(double alpha) const {
    double q;
    if (m_type ==0) {
        int index = this->Quantile_Index(alpha);
        q = m_X[index];
    }
    return q;
}

/**
 * Compute Value-at-Risk
 */
double RandomVar::VaR(double alpha) const {
    return Quantile(1.0 - alpha);
}

/**
 * Compute Expected Shortfall
 */
double RandomVar::ExpectedShortFall(double alpha) const {
    double es = 0;
    if (m_type == 0) {
        int iVaR = this->Quantile_Index(alpha);
        for (int k = iVaR; k < m_P.size(); k++) {
            es += m_P[k] * m_X[k];
        }
        es /= alpha;
    }
    return es;
}

/**
 * Compute Expeedance Probability
 */
double RandomVar::ExceedanceProbability(int index) const {
    double ep = 0;
    if (m_type == 0){
        for (size_t k = index; k < m_P.size(); k++)
            ep += m_P[k];
    }
    return ep;
}

/**
 * Compute Mean Excess
 */
double RandomVar::MeanExcess(int index) const {
    double mx = 0;
    if (m_type == 0) {
        double alpha = ExceedanceProbability(index);
        for (size_t k = index; k < m_P.size(); k++)
            mx += m_P[k] * m_X[k];
        mx /= alpha;
    }
    return mx;
}

/**
 * overload << operator
 */
std::ostream &operator<<(std::ostream &os, const RandomVar &R) {
    std::ostringstream out;
    for (size_t k = 0; k < R.size(); k++)
        out << R.getX(k) << "\t" << R.getP(k) << "\t" << R.getC(k) << std::endl;
    return os << out.str();
};

/**
 * Print a representation
 */
void RandomVar::Print() {
    if (this->m_type == 1) {
        for (size_t s = 0; s < this->m_S.size(); s++) {
            std::cout << s << "\t" << this->m_S[s] << std::endl;
        }
    } else if (this->m_type == 0) {
        for (size_t s = 0; s < this->m_X.size(); s++) {
            std::cout << s << "\t" << this->m_X[s] << "\t" << this->m_P[s] << "\t" << this->m_C[s] << std::endl;
        }
    }
}

/**
 * Read random variable from JSON
 * @param fileName
 */
void RandomVar::ReadFromJSON(const std::string &filename) {

    // Random variable data are an array of objects
    //[
    // {"value": 1, "probability" : 0.2, "cumulative" : 0.2},
    // {"value": 2, "probability" : 0.2, "cumulative" : 0.4},
    // {"value": 3, "probability" : 0.2, "cumulative" : 0.6},
    // {"value": 4, "probability" : 0.2, "cumulative" : 0.8},
    // {"value": 5, "probability" : 0.2, "cumulative" : 1.0}
    //]

    Poco::JSON::Parser Parser;
    const char *cstr = filename.c_str();
    std::ifstream t(cstr);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string json = buffer.str();
    // Parse the JSON and get the Results
    Parser.parse(json);
    Poco::Dynamic::Var ParsedJsonResult = Parser.result();

    Poco::JSON::Array::Ptr arr = ParsedJsonResult.extract<Poco::JSON::Array::Ptr>();
    size_t size = arr->size();
    std::cout << "Reading " << size << " records." << std::endl;

    m_P.resize(size);
    m_C.resize(size);
    m_X.resize(size);

    // Individual data rows
    Poco::JSON::Object::Ptr object;
    for (size_t i = 0; i < size; i++) {
        object = arr->getObject(i);
        this->setX(i, object->getValue<double>("value"));
        this->setP(i, object->getValue<double>("probability"));
        this->setC(i, object->getValue<double>("cumulative"));
    }
}
