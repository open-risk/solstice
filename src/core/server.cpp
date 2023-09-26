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

#include <Poco/SimpleFileChannel.h>
#include <Poco/ConsoleChannel.h>
#include "Poco/Environment.h"
#include "Poco/Path.h"
#include <Poco/StreamCopier.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <string>
#include <vector>

#include "core/simulation.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class SolsticeRequestHandler : public HTTPRequestHandler {
public:
    virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) {

        Application& app = Application::instance();
        app.logger().information("Request from " + req.clientAddress().toString());

        std::string executable = "SolsticeServer";
        std::string configuration = "DryRun";
        char** arg_strings[2];
        arg_strings[0] = (char **) executable.c_str();
        arg_strings[1] = (char **) configuration.c_str();

        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");

        std::string recv_string;
        Poco::StreamCopier::copyToString(req.stream(), recv_string);

        ostream &out = resp.send();
        out << "<h3>Solstice Server Response</h3>"
            << "<p>Count: " << ++count << "</p>"
            << "<p>Host: " << req.getHost() << "</p>"
            << "<p>Method: " << req.getMethod() << "</p>"
            << "<p>URI: " << req.getURI() << "</p>"
            << "<p>POST Payload: " << recv_string << "</p>";
        out.flush();

        if (req.getMethod() == "GET") {
            app.logger().information("GET Request Processing");
        }
        else if (req.getMethod() == "POST") {
            app.logger().information("POST Request Processing");
            Poco::LogStream logstream(app.logger());
            Simulation engine(OPERATING_MODE, recv_string,logstream, reinterpret_cast<char **>(arg_strings));
            engine.run(logstream);
        }


//        logstream.information() << "> Read and Parsed Configuration Data from API " << std::endl;

        cout << endl
             << "Response sent for count=" << count
             << " and URI=" << req.getURI() << endl;
    }

private:
    static int count;
};

int SolsticeRequestHandler::count = 0;


class SolsticeRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    virtual HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &) {
        return new SolsticeRequestHandler;
    }
};

class SolsticeServer : public ServerApplication {

public:
    SolsticeServer() : _helpRequested(false) {
    }

    ~SolsticeServer() {
    }

protected:

    void initialize(Application &self) {
        loadConfiguration(); // load default configuration files, if present
        ServerApplication::initialize(self);
    }

    void uninitialize() {
        ServerApplication::uninitialize();
    }

    void defineOptions(OptionSet &options) {
        ServerApplication::defineOptions(options);

        options.addOption(
                Option("help", "h", "display help information on command line arguments")
                        .required(false)
                        .repeatable(false));
    }

    void handleOption(const std::string &name, const std::string &value) {
        ServerApplication::handleOption(name, value);

        if (name == "help")
            _helpRequested = true;
    }

    void displayHelp() {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A Solstice server for performing economic network simulations.");
        helpFormatter.format(std::cout);
    }

    int main(const vector<string> &args) {

        if (_helpRequested) {
            displayHelp();
        } else {
            HTTPServer server(new SolsticeRequestHandlerFactory, ServerSocket(9090), new HTTPServerParams);

            server.start();
            cout << endl << "Solstice HTTP Server started" << endl;
            cout << endl << "Listening at port 9090" << endl;
            waitForTerminationRequest();  // wait for CTRL-C or kill
            cout << endl << "Solstice HTTP Server Shutting down..." << endl;
            server.stop();

            // Shutdown logging system
            Poco::Logger::shutdown();
        }
        return Application::EXIT_OK;
    }

private:
    bool _helpRequested;
};

int main(int argc, char **argv) {

    //
    // Start logging
    //

    // Setup console and file log channels
    Poco::AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);

    // Construct path to logfile
    std::string logstreamname;
    logstreamname = "Logs/configuration_" + get_current_time() + ".log";

    // Configure file logging
    // Must have Log directory writeable
    std::string path = Poco::Path::current();
    pChannel->setProperty("path", path + logstreamname);
    pChannel->setProperty("rotation", "never");

    Poco::Logger &logger = Poco::Logger::get("Configuration_Logger"); // inherits root channel
    logger.setLevel(LOG_LEVEL);
    Poco::LogStream logstream(logger);

    SolsticeServer app;
    return app.run(argc, argv);
}