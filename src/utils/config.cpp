#include <iostream>
#include <fstream>
#include <sstream>

#include "utils/config.hpp"
#include "utils/log.hpp"

const std::string filename = "static/server/config.txt";

bool read(const std::string &filename, std::map<std::string, std::string> &read_config_map) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LogErr("Failed to open configuration file: " + filename);
        std::cerr << "Failed to open configuration file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

    std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Remove leading/trailing whitespace from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Store the key-value pair in the config structure
            read_config_map[key] = value;
        }
    }

    file.close();
    return true;
}

bool Config::load_configuration() {
    std::map<std::string, std::string> read_config_map;

    if (read(filename, read_config_map)) {
        for (auto &setting : conf_s.int_settings)
            if (auto it = read_config_map.find(setting.first); it != read_config_map.end()) {
                try {
                    setting.second = std::stoi(it->second);
                } catch (std::exception &e) {
                    LogErr("error when parse config " + std::string(e.what()));
                    std::cerr << "error when parse config " << e.what() << std::endl;
                    return false;
                }
            }

        for (auto &setting : conf_s.string_settings)
            if (auto it = read_config_map.find(setting.first); it != read_config_map.end()) {
                try {
                    setting.second = it->second;
                } catch (std::exception &e) {
                    LogErr("error when parse config " + std::string(e.what()));
                    std::cerr << "error when parse config " << e.what() << std::endl;
                    return false;
                }
            }

        return true;
    } else {
        return false;
    }

    return true;
}
