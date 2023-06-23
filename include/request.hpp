#pragma once

#include <iostream>
#include <array>
#include <string>
#include <unordered_map>


enum RequestMethod {
    GET,
    POST,
    PUT,
    DELETE
};

constexpr std::array<const char*, 4> METHOD_STR = {
    "GET",
    "POST",
    "PUT",
    "DELETE"
};

class HttpRequest {
public:
    HttpRequest(std::string _request) : request(_request) {}

    void parse() {
        get_request_method();
        get_request_path();
        get_request_queries();
        get_request_headers();
        get_request_body();
    }

    void show_request() {
        std::cout << "method " << method << std::endl;
        std::cout << "path " << path << std::endl;
        for (auto t : queries)
            std::cout << t.first << " " << t.second << std::endl;
        std::cout << "headers " << std::endl;
        for (auto t : headers)
            std::cout << t.first << " " << t.second << std::endl;
        std::cout << "body " << body << std::endl;
    }

    const RequestMethod &get_method() const;
    const std::string &get_path() const;

private:
    std::string get_request_method();
    std::string get_request_path();
    std::unordered_map<std::string, std::string> get_request_queries();
    std::unordered_map<std::string, std::string> get_request_headers();
    std::string get_request_body();

    const std::string request;
    RequestMethod method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> queries;
    std::string body;
};
