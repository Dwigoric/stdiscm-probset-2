#include "util.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

std::tuple<std::string, unsigned short> parse_line(const std::string &line) {
    const size_t pos = line.find('=');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid line format");
    }

    // Remove leading and trailing whitespace
    std::string trimmed_line = line;
    trimmed_line.erase(0, trimmed_line.find_first_not_of(" \t"));
    trimmed_line.erase(trimmed_line.find_last_not_of(" \t") + 1);
    std::string trimmed_key = trimmed_line.substr(0, pos);
    trimmed_key.erase(0, trimmed_key.find_first_not_of(" \t"));
    trimmed_key.erase(trimmed_key.find_last_not_of(" \t") + 1);
    std::string trimmed_value = trimmed_line.substr(pos + 1);
    trimmed_value.erase(0, trimmed_value.find_first_not_of(" \t"));
    trimmed_value.erase(trimmed_value.find_last_not_of(" \t") + 1);

    // Check if the key is valid
    if (trimmed_key != "n" && trimmed_key != "t" && trimmed_key != "h" && trimmed_key != "d" &&
        trimmed_key != "t1" && trimmed_key != "t2") {
        throw std::invalid_argument("Unrecognized configuration key: " + trimmed_key);
    }
    // Check if the value is a number
    if (trimmed_value.empty() || !std::ranges::all_of(trimmed_value, ::isdigit)) {
        throw std::invalid_argument("Value for configuration key " + trimmed_key + " is not a number");
    }
    // Convert the value to unsigned short
    auto value = static_cast<unsigned short>(std::stoi(trimmed_value));
    return {trimmed_key, value};
}

std::unordered_map<std::string, unsigned short> read_config() {
    std::unordered_map<std::string, unsigned short> config;

    if (std::ifstream file("config.txt"); file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::tuple<std::string, unsigned short> config_line = parse_line(line);
            const auto &[key, value] = config_line;

            if (config.contains(key)) {
                throw std::invalid_argument("Duplicate configuration key: " + key);
            }

            config.insert({key, value});
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open config.txt");
    }

    return config;
}
