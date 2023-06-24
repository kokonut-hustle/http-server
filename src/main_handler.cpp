#include "main_handler.hpp"
#include "common.hpp"
#include "log.hpp"

HttpResponse MainHandler::handle(HttpRequest req) {
    HttpResponse resp(200);
    resp.set_body("Hello World!!! I am human");
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
