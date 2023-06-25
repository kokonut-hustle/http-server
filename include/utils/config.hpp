#pragma once
#include <map>
#include <string>

namespace Config {
    template<typename T>
    using Settings = std::map<std::string, T>;

    struct Configuration {
        Settings<int> int_settings;
        Settings<std::string> string_settings;
    };

    bool load_configuration();
}
