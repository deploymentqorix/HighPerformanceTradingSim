#pragma once
#include <string>
#include <unordered_map>

class Config {
public:
    Config();  // default constructor
    Config(const std::string& filepath);  // <-- add this declaration

    std::string get(const std::string& key) const;

    int max_order_size; // <-- this field needed because main.cpp uses it

private:
    std::unordered_map<std::string, std::string> values;
};
