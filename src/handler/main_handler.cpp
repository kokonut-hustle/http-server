#include "handler/main_handler.hpp"
#include "utils/log.hpp"

HttpResponse MainHandler::handle(const HttpRequest &req) {
    HttpResponse resp(200);
    resp.set_body("Hello World!!! I am human");
    resp.set_header("Content-Type", "text/plain");
    LogInfo("Request was served successfully: " + req.get_path());
    return resp;
}
