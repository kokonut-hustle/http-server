#include "utils/response.hpp"

void HttpResponse::set_header(const std::string& name, const std::string& value) {
    headers_[name] = value;
}

void HttpResponse::set_body(const std::string& body) {
    body_ = body;
}

std::string HttpResponse::to_string() const {
    std::string response;

    // Append the status line
    response += "HTTP/1.1 " + std::to_string(statusCode_) + " " + get_status_message(statusCode_) + "\r\n";

    // Append the headers
    for (const auto& header : headers_) {
        response += header.first + ": " + header.second + "\r\n";
    }

    // Append the Content-Length header
    response += "Content-Length: " + std::to_string(body_.size()) + "\r\n";

    // Append the empty line indicating the end of headers
    response += "\r\n";

    // Append the response body
    response += body_;

    return response;
}

std::string HttpResponse::get_status_message(int statusCode) const {
    return status.at(statusCode);
}
