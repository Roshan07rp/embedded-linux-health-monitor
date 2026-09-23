#include "temperature_monitor.h"
#include <filesystem>
#include <fstream>
#include <string>

double get_temperature_celsius() {
    namespace fs = std::filesystem;
    const fs::path root("/sys/class/thermal");

    if (!fs::exists(root)) return -1.0;

    for (const auto& entry : fs::directory_iterator(root)) {
        const auto temp_file = entry.path() / "temp";
        if (!fs::exists(temp_file)) continue;

        std::ifstream file(temp_file);
        long long millidegrees = 0;
        if (file >> millidegrees) {
            return static_cast<double>(millidegrees) / 1000.0;
        }
    }

    return -1.0;
}
