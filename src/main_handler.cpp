#include "main_handler.hpp"
#include "log.hpp"

HttpResponse MainHandler::handle(HttpRequest req) {
    HttpResponse resp(200);
    resp.set_body("Hello World!!! I am human");
    resp.set_header("Content-Type", "text/plain");
    LogInfo("Request was served successfully: " + req.get_path());
    return resp;
}
