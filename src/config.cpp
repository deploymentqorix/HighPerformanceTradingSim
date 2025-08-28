#include "config.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (!(iss >> key >> value)) continue;

        if (key == "max_order_size") {
            max_order_size = std::stoi(value);
        } else if (key == "csv_filepath") {
            csv_filepath = value;
        }
    }
}
