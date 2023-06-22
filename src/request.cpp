#include "request.hpp"

const std::string &HttpRequest::get_path() const {
    return path;
}

const RequestMethod &HttpRequest::get_method() const {
    return method;
}

void HttpRequest::set_req_id() {
    req_id = std::string(METHOD_STR[method]) + " " + path;
}

const std::string &HttpRequest::get_req_id() const {
    return req_id;
}

std::string HttpRequest::get_request_method() {
    std::string method_str;

    // Find the end of the method (indicated by a space)
    size_t method_end = request.find(' ');

    if (method_end != std::string::npos) {
        // Extract the method substring
        method_str = request.substr(0, method_end);

        // Validate the extracted method
        if (method_str == "GET") {
            method = RequestMethod::GET;
        } else if (method_str == "POST") {
            method = RequestMethod::POST;
        } else if (method_str == "PUT") {
            method = RequestMethod::PUT;
        } else if (method_str == "DELETE") {
            method = RequestMethod::DELETE;
        } else {
            throw std::runtime_error("Invalid request method: " + method_str);
        }
    } else {
        throw std::runtime_error("Invalid request format");
    }

    return method_str;
}

std::string HttpRequest::get_request_path() {
    // Find the start of the path (indicated by the first space character after the method)
    size_t method_end = request.find(' ');
    if (method_end == std::string::npos) {
        throw std::runtime_error("Invalid request format: no method found");
    }

    size_t path_start = request.find_first_not_of(' ', method_end + 1);
    if (path_start == std::string::npos) {
        throw std::runtime_error("Invalid request format: no path found");
    }

    // Find the end of the path (indicated by the ampersand or the next space character or end of line)
    size_t path_end = request.find('&', path_start);
    if (path_end == std::string::npos) {
        path_end = request.find(' ', path_start);
        if (path_end == std::string::npos) {
            path_end = request.size();
        }
    }

    // Extract the path substring
    path = request.substr(path_start, path_end - path_start);

    return path;
}

std::unordered_map<std::string, std::string> HttpRequest::get_request_queries() {
    // Find the position of the query string in the URL
    size_t queries_start = path.find('?');
    if (queries_start != std::string::npos) {
        // Extract the query string
        std::string queries_str = path.substr(queries_start + 1);

        // Split the query string into individual key-value pairs
        size_t pos = 0;
        while (pos != std::string::npos) {
            // Find the position of the next '&' character
            size_t ampersand_pos = queries_str.find('&', pos);

            // Extract the key-value pair
            std::string key_val_pair;
            if (ampersand_pos != std::string::npos) {
                key_val_pair = queries_str.substr(pos, ampersand_pos - pos);
                pos = ampersand_pos + 1;
            } else {
                key_val_pair = queries_str.substr(pos);
                pos = std::string::npos;
            }

            // Split the key-value pair into key and value
            size_t equal_pos = key_val_pair.find('=');
            if (equal_pos != std::string::npos) {
                std::string key = key_val_pair.substr(0, equal_pos);
                std::string value = key_val_pair.substr(equal_pos + 1);

                // URL-decode the key and value if necessary

                // Store the key-value pair in the result map
                queries[key] = value;
            }
        }
    }

    return queries;
}

std::unordered_map<std::string, std::string> HttpRequest::get_request_headers() {
    // Find the position of the end of the request line
    size_t request_line_end_pos = request.find("\r\n");
    if (request_line_end_pos == std::string::npos) {
        throw std::runtime_error("Invalid request format: no request line found");
    }

    // Skip the request line
    size_t start_pos = request_line_end_pos + 2; // Move past the request line

    // Find the end of the headers section (indicated by an empty line)
    size_t end_pos = request.find("\r\n\r\n", start_pos);
    if (end_pos == std::string::npos) {
        throw std::runtime_error("Invalid request format: no headers found");
    }

    // Parse each header line and store in the headers map
    while (start_pos < end_pos) {
        size_t colon_pos = request.find(":", start_pos);
        if (colon_pos == std::string::npos) {
            throw std::runtime_error("Invalid header format: missing colon");
        }

        std::string header_name = request.substr(start_pos, colon_pos - start_pos);
        std::string header_value = request.substr(colon_pos + 2, request.find("\r\n", colon_pos) - colon_pos - 2);
        headers[header_name] = header_value;

        start_pos = request.find("\r\n", start_pos) + 2;
    }

    return headers;
}

std::string HttpRequest::get_request_body() {
    // Find the start of the body (indicated by "\r\n\r\n")
    size_t body_start = request.find("\r\n\r\n");

    if (body_start != std::string::npos) {
        // Extract the body substring
        body = request.substr(body_start + 4);
    }

    return body;
}
