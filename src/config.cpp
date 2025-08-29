#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>  // if you use JSON parsing
using json = nlohmann::json;

Config::Config() {}

Config::Config(const std::string& filepath) {
    std::ifstream f(filepath);
    if (f.is_open()) {
        json j;
        f >> j;

        // Example: read max_order_size
        if (j.contains("max_order_size"))
            max_order_size = j["max_order_size"];
        else
            max_order_size = 1000; // default
    } else {
        max_order_size = 1000; // fallback
    }
}

std::string Config::get(const std::string& key) const {
    auto it = values.find(key);
    if (it != values.end())
        return it->second;
    return "";
}
