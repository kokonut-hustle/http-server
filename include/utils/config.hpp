#pragma once
#include <map>
#include <string>

template<typename T>
using Settings = std::map<std::string, T>;

class Config {
public:
    struct ConfigStruct {
        Settings<int> int_settings;
        Settings<std::string> string_settings;
    } conf_s;

    bool load_configuration();
};
