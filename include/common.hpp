#pragma once
#include <string>
#include <algorithm>

namespace {
    bool is_keep_alive(const std::string &inp) {
        if (inp.empty()) {
            // Connection header not found, assume keep-alive by default
            return true;
        }

        std::string temp(inp);
        std::transform(inp.begin(), inp.end(), temp.begin(), [](unsigned char c){
            return std::tolower(c);
        });

        return (temp == "keep-alive");
    }
}
