#include <iostream>
#include <fstream>
#include <sstream>

#include "config.hpp"

bool Config_t::read(const std::string &filename) {
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
            this->values[key] = value;
        }
    }

    file.close();
    return true;
}
