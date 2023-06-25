#include "param_handler/main_param_handler.hpp"
#include "handler/error_page_handler.hpp"
#include "utils/log.hpp"

HttpResponse MainParamHandler::handle(const HttpRequest &req,
    const std::unordered_map<std::string, std::string> &params) {
    if (auto it = params.find("name"); it == params.end()) {
        LogInfo("Can't find param \"name\" from path: " + req.get_path());
        return ErrorPageHandler::handle();
    } else {
        HttpResponse resp(200);
        resp.set_body("Hello World!!! I am " + params.at("name"));
        resp.set_header("Content-Type", "text/plain");

        LogInfo("Request was served successfully: " + req.get_path());
        return resp;
    }
}
