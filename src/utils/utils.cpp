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

#include "utils.h"

using namespace std;
using namespace Eigen;

double round_to_n_digits(double x, int n) {
    if (x != 0.0) {
        double scale = pow(10.0, n);
        return round(x * scale) / scale;
    } else {
        return 0.0;
    }
}

std::string get_json(Poco::URI uri, std::string APItoken, Poco::LogStream &logstream) {

#if SSL_MODE == 0
    Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
#elif SSL_MODE == 1
    Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") );
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
#elif SSL_MODE==2
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
#endif

    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
    std::string jwt_string = "JWT " + APItoken;
    req.add("Authorization", jwt_string);
    session.sendRequest(req);
    // get server response
    Poco::Net::HTTPResponse res;
    std::istream &is = session.receiveResponse(res);

    std::stringstream response_data;
    Poco::StreamCopier::copyStream(is, response_data);
    std::string json = response_data.str();

    if (LOG_LEVEL >= 2) {
        logstream.information() << ">> 3 | Server Response " << std::endl;
        logstream.information() << ">> 3 | " << res.getStatus() << " " << res.getReason() << std::endl;
        if (LOG_LEVEL >= 3) {
            logstream.information() << json << std::endl;
        }
    }

    return json;
}


void Col_to_JSON_file(VectorXd &A, const std::string &format, const std::string &filename) {



    int Precision = 5;
    std::stringstream output;
    Poco::JSON::Array A_Serialized;
    for (unsigned int i = 0; i < A.cols() * A.rows(); i++) {
        A_Serialized.add(round_to_n_digits(A[i], Precision));
    }
    Poco::JSON::Stringifier::stringify(A_Serialized, output);
    std::ofstream out(filename.c_str());
    out.precision(5);
    out << output.str() << std::endl;
}

/**
 * Serialiase an Eigen matrix into JSON
 * @param A
 * @return
 */
Poco::JSON::Object Mat_to_JSON(MatrixXd &A) {

    int Precision = 5;
    Poco::JSON::Object A_Serialized;
    Poco::JSON::Array A_Column;
    std::stringstream ss;

    // outer loop (inner index)
    for (unsigned int i = 0; i < A.rows(); i++) {
        // inner loop (outer index)
        A_Column.clear();
        for (unsigned int j = 0; j < A.cols(); j++) {
            A_Column.add(round_to_n_digits(A(i, j), Precision));
        }
        ss << i;
        std::string Row_Index = ss.str();
        ss.str("");
        A_Serialized.set(Row_Index, A_Column);
    }
    return A_Serialized;
}

void Mat_to_JSON_file(MatrixXd &A, const std::string &format, const std::string &filename) {

    int Precision = 5;
    std::stringstream output;
    Poco::JSON::Object A_Serialized;
    Poco::JSON::Array A_Column;
    std::stringstream ss;
    // outer loop (inner index)
    for (unsigned int i = 0; i < A.rows(); i++) {
        // inner loop (outer index)
        A_Column.clear();
        for (unsigned int j = 0; j < A.cols(); j++) {
            A_Column.add(round_to_n_digits(A(i, j), Precision));
        }
        ss << i;
        std::string Row_Index = ss.str();
        ss.str("");
        A_Serialized.set(Row_Index, A_Column);
    }
    Poco::JSON::Stringifier::stringify(A_Serialized, output);
    std::ofstream out(filename.c_str());
    out.precision(5);
    out << output.str() << std::endl;
}

void Cub_to_JSON_file(Tensor<double, 3> &A, const std::string &format, const std::string &filename) {

    int Precision = 5;
    std::stringstream output;
    cout << format << std::endl;

    if (format == "dict") {
        // Serializing a Cube object to JSON
        // Object of objects 
        // { "0" : {  }, "1" : {  } ... ]    
        Poco::JSON::Object A_Serialized;
        // Rating threshold object
        // { "0" : [T, ... T], "1" : [T, ... T]    
        Poco::JSON::Object A_Matrix;
        // Array in period sequence    
        Poco::JSON::Array A_Column;
        std::stringstream ss;
        // outer loop (inner index)
        for (unsigned int i = 0; i < A.dimension(1); i++) {
            A_Matrix.clear();
            // middle loop (middle index)
            for (unsigned int j = 0; j < A.dimension(2); j++) {
                // for (unsigned int j = 0; j < A.cols(); j++) {
                // inner loop (outer index)
                A_Column.clear();
                for (unsigned int k = 0; k < A.dimension(3); k++) {
                    // for (unsigned int k = 0; k < A.n_slices; k++) {
                    A_Column.add(round_to_n_digits(A(i, j, k), Precision));
                }
                ss << j;
                std::string Col_Index = ss.str();
                ss.str("");
                A_Matrix.set(Col_Index, A_Column);
            }
            ss << i;
            std::string Row_Index = ss.str();
            ss.str("");
            A_Serialized.set(Row_Index, A_Matrix);
        }
        Poco::JSON::Stringifier::stringify(A_Serialized, output);
        std::ofstream out(filename.c_str());
        out.precision(5);
        out << output.str() << std::endl;
    } else if (format == "array") {
        // Serializing a Cube object to JSON
        // Object of objects 
        // { "0" : {  }, "1" : {  } ... ]    
        Poco::JSON::Array A_Serialized;
        // Rating threshold list
        // [ [T, ... T],  [T, ... T] ]    
        Poco::JSON::Array A_Matrix;
        // Array in period sequence    
        Poco::JSON::Array A_Column;
        // outer loop (inner index)
        // for (unsigned int k = 0; k < A.n_slices; k++) {
        for (unsigned int k = 0; k < A.dimension(3); k++) {
            A_Matrix.clear();
            // From Level
            // for (unsigned int i = 0; i < A.cols(); i++) {
            for (unsigned int i = 0; i < A.dimension(2); i++) {
                A_Column.clear();
                // middle loop (middle index)
                // To Level
                //for (unsigned int j = 0; j < A.rows(); j++) {
                for (unsigned int j = 0; j < A.dimension(1); j++) {
                    // inner loop (outer index)
                    A_Column.add(round_to_n_digits(A(i, j, k), Precision));
                }
                A_Matrix.add(A_Column);
            }
            A_Serialized.add(A_Matrix);
        }

        Poco::JSON::Stringifier::stringify(A_Serialized, output, 2);
        std::ofstream out(filename.c_str());
        out.precision(5);
        out << output.str() << std::endl;
    }


}