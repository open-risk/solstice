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

#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <Poco/LogStream.h>
#include <Poco/FileChannel.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Path.h>
#include <Poco/StreamCopier.h>
#include <Poco/Environment.h>

#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

std::string get_json(Poco::URI uri, std::string APItoken, Poco::LogStream &logstream);

void Col_to_JSON_file(Eigen::VectorXd &A, const std::string &format, const std::string &filename);

void Mat_to_JSON_file(Eigen::MatrixXd &A, const std::string &format, const std::string &filename);

void Cub_to_JSON_file(Eigen::Tensor<double, 3> &A, const std::string &format, const std::string &filename);

Poco::JSON::Object Mat_to_JSON(Eigen::MatrixXd &A);

double round_to_n_digits(double x, int n);

#endif /* UTILS_H */

