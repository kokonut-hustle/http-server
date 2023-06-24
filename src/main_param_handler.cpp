#include "main_param_handler.hpp"
#include "common.hpp"
#include "error_page_handler.hpp"
#include "log.hpp"

HttpResponse MainParamHandler::handle(HttpRequest req,
    const std::unordered_map<std::string, std::string> &params) {
    if (auto it = params.find("name"); it == params.end()) {
        LogInfo("Can't find param \"name\" from path: " + req.get_path());
        return ErrorPageHandler::handle();
    } else {
        HttpResponse resp(200);
        resp.set_body("Hello World!!! I am " + params.at("name"));
        resp.set_header("Content-Type", "text/plain");
        if (const auto it = req.get_headers().find("Connection");
            it != req.get_headers().end() && is_keep_alive(it->second)) {
            resp.set_header("Connection", "keep-alive");
            this->keep_alive = true;
        } else {
            this->keep_alive = false;
        }

        LogInfo("Request was served successfully: " + req.get_path());
        return resp;
    }
}
