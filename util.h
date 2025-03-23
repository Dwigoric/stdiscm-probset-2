#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <unordered_map>

std::tuple<std::string, unsigned short> parse_line(const std::string &line);

std::unordered_map<std::string, unsigned short> read_config();

#endif //UTIL_H
