#include "handler/error_page_handler.hpp"

HttpResponse &ErrorPageHandler::handle() {
    static HttpResponse resp(400);
    return resp;
}
