#pragma once

#include <string>
#include <map>

class HttpResponse {
public:
    explicit HttpResponse(int statusCode)
        : statusCode_(statusCode) {
    }

    void set_header(const std::string& name, const std::string& value);

    void set_body(const std::string& body);

    std::string to_string() const;

private:
    const std::map<int, std::string> status = {
        {200, "OK"},
        {400, "Bad Request"},
        {404, "Not Found"},
        {500, "Internal Server Error"},
        {-1,  "Unknown Status"}
    };

    int statusCode_;
    std::map<std::string, std::string> headers_;
    std::string body_;

    std::string get_status_message(int statusCode) const;
};
