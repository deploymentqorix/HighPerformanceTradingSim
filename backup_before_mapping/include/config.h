#pragma once
#include <string>

class Config {
public:
    explicit Config(const std::string& filepath);
    int max_order_size;
    std::string csv_filepath;
};
