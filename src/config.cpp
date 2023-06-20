#include <iostream>
#include <fstream>
#include <sstream>

#include "config.hpp"

namespace Config {
    Settings<int> int_settings = {{"setting1", 1}};
    Settings<std::string> string_settings = {{"setting1", "val1"}};
    Configuration config{std::move(int_settings), std::move(string_settings)};
    const std::string filename = "config.txt";

    bool read(const std::string &filename, std::map<std::string, std::string> &config_map) {
        std::ifstream file(filename);
        if (!file.is_open()) {
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
                key.erase(key.find_first_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_first_not_of(" \t") + 1);

                // Store the key-value pair in the config structure
                config_map[key] = value;
            }
        }

        file.close();
        return true;
    }

    bool load_configuration() {
        std::map<std::string, std::string> config_map;

        if (read(filename, config_map)) {
            for (auto &setting : config.int_settings)
                if (auto it = config_map.find(setting.first); it != config_map.end()) {
                    try {
                        setting.second = std::stoi(it->second);
                    } catch (std::exception &e) {
                        std::cerr << "error when parse config " << e.what() << std::endl;
                        return false;
                    }
                }

            for (auto &setting : config.string_settings)
                if (auto it = config_map.find(setting.first); it != config_map.end()) {
                    setting.second = it->second;
                }

            return true;
        } else {
            return false;
        }

        return true;
    }
}
