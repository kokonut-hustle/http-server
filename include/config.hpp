#pragma once
#include <string>
#include <map>

class Config_t {
    using Config = std::map<std::string, std::string>;
public:
    bool read(const std::string &filename);
private:
    Config values;
};
