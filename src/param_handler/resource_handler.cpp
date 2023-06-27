#include "param_handler/resource_handler.hpp"
#include "handler/error_page_handler.hpp"
#include "utils/log.hpp"

HttpResponse ResourceHandler::handle(const HttpRequest &req,
    const std::unordered_map<std::string, std::string> &params) {
    if (auto it = params.find("resource_name"); it == params.end()) {
        LogInfo("Can't find param \"resource_name\" from path: " + req.get_path());
        return ErrorPageHandler::handle();
    } else {
        try {
            std::string file_content = read_file_content("static/" + it->second);

            HttpResponse resp(200);
            resp.set_body(file_content);
            resp.set_header("Content-Type", "text/html");

            LogInfo("Request was served successfully: " + req.get_path());
            return resp;
        } catch (std::exception &e) {
            std::cerr << "Failed to open file: " << e.what() << std::endl;
            return ErrorPageHandler::handle();
        }
    }
}

std::string ResourceHandler::read_file_content(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file)
        throw std::runtime_error("Can't open file: " + filename);

    std::ifstream::pos_type file_size = file.tellg();
    std::vector<char> buffer(file_size);
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), file_size);

    return std::string(buffer.data(), file_size);
}
