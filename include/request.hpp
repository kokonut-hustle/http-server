#pragma once

#include <iostream>
#include <string>
#include <map>

class HttpRequest {
public:
    HttpRequest(std::string _request) : request(_request) {}

    void parse() {
        get_request_method();
        get_request_path();
        get_request_headers();
        get_request_body();
    }

    void show_request() {
        std::cout << "method " << method << std::endl;
        std::cout << "path " << path << std::endl;
        std::cout << "headers " << std::endl;
        for (auto t : headers)
            std::cout << t.first << " " << t.second << std::endl;
        std::cout << "body " << body << std::endl;
    }

private:
    enum RequestMethod {
        GET,
        POST,
        PUT,
        DELETE
    };

    std::string get_request_method();
    std::string get_request_path();
    std::map<std::string, std::string> get_request_headers();
    std::string get_request_body();

    const std::string request;
    RequestMethod method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
};
