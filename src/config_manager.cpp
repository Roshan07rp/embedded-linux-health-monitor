#include "config_manager.h"
#include <fstream>
#include <regex>
#include <sstream>

namespace {
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool get_number(const std::string& text, const std::string& key, double& value) {
    std::regex pattern("\"" + key + "\"\\s*:\\s*(-?[0-9]+(?:\\.[0-9]+)?)");
    std::smatch match;
    if (!std::regex_search(text, match, pattern)) return false;
    value = std::stod(match[1].str());
    return true;
}

bool get_int(const std::string& text, const std::string& key, int& value) {
    double temp = 0;
    if (!get_number(text, key, temp)) return false;
    value = static_cast<int>(temp);
    return true;
}

bool get_string(const std::string& text, const std::string& key, std::string& value) {
    std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
    std::smatch match;
    if (!std::regex_search(text, match, pattern)) return false;
    value = match[1].str();
    return true;
}
}

bool load_config(const std::string& path, HealthConfig& config) {
    const std::string text = read_file(path);
    if (text.empty()) return false;

    get_int(text, "interval_seconds", config.interval_seconds);
    get_number(text, "cpu_warning_percent", config.cpu_warning_percent);
    get_number(text, "cpu_critical_percent", config.cpu_critical_percent);
    get_number(text, "memory_warning_percent", config.memory_warning_percent);
    get_number(text, "memory_critical_percent", config.memory_critical_percent);
    get_number(text, "disk_warning_percent", config.disk_warning_percent);
    get_number(text, "disk_critical_percent", config.disk_critical_percent);
    get_string(text, "disk_path", config.disk_path);
    get_number(text, "temperature_warning_celsius", config.temperature_warning_celsius);
    get_number(text, "temperature_critical_celsius", config.temperature_critical_celsius);
    get_string(text, "network_interface", config.network_interface);
    get_string(text, "network_test_host", config.network_test_host);
    get_string(text, "log_file", config.log_file);

    std::regex service_pattern(
        "\\{\\s*\"name\"\\s*:\\s*\"([^\"]+)\"\\s*,\\s*\"auto_restart\"\\s*:\\s*(true|false)\\s*\\}"
    );
    for (std::sregex_iterator it(text.begin(), text.end(), service_pattern), end;
         it != end; ++it) {
        ServiceConfig service;
        service.name = (*it)[1].str();
        service.auto_restart = ((*it)[2].str() == "true");
        config.services.push_back(service);
    }

    return true;
}
