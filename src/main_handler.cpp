#include "main_handler.hpp"

HttpResponse MainHandler::handle(HttpRequest req) {
    HttpResponse resp(200);
    resp.set_body("Hello World!!! I am human");
    resp.set_header("Content-Type", "text/plain");
    resp.set_header("Connection", "close");

    return resp;
}
