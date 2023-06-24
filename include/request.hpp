#pragma once

#include <iostream>
#include <array>
#include <string>
#include <unordered_map>

#include "log.hpp"

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

    bool parse() {
        try {
            get_request_method();
            get_request_path();
            get_request_queries();
            get_request_headers();
            get_request_body();
            return true;
        } catch (const std::exception &e) {
            LogErr("Failed to parse request: " + path);
            return false;
        }
    }

    const RequestMethod &get_method() const;
    const std::string &get_path() const;
    const std::unordered_map<std::string, std::string> &get_headers() const;

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
