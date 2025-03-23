#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <random>
#include <string>
#include <unordered_map>

inline std::default_random_engine generator(
    static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())
);

std::tuple<std::string, unsigned short> parse_line(const std::string &line);

std::unordered_map<std::string, unsigned short> read_config();

#endif //UTIL_H
