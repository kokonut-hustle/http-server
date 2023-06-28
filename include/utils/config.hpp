#pragma once
#include <map>
#include <string>

#include "constants.hpp"

class Config {
public:
    struct ConfigStruct {
        Settings<int> int_settings;
        Settings<std::string> string_settings;
    } conf_s;

    bool load_configuration();
};
