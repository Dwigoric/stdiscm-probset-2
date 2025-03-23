#include "util.h"

#include <chrono>
#include <string>
#include <unordered_map>

int main() {
    const std::unordered_map<std::string, unsigned short> config = read_config();

    // Check if all keys are present
    for (const std::string required_keys[] = {"n", "t", "h", "d", "t1", "t2"}; const auto &key: required_keys) {
        if (!config.contains(key)) {
            throw std::invalid_argument("Missing configuration key: " + key);
        }
    }

    return 0;
}
