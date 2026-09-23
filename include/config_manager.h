#pragma once
#include <string>
#include <vector>

struct ServiceConfig {
    std::string name;
    bool auto_restart{false};
};

struct HealthConfig {
    int interval_seconds{10};
    double cpu_warning_percent{80};
    double cpu_critical_percent{95};
    double memory_warning_percent{80};
    double memory_critical_percent{90};
    double disk_warning_percent{80};
    double disk_critical_percent{90};
    std::string disk_path{"/"};
    double temperature_warning_celsius{70};
    double temperature_critical_celsius{85};
    std::string network_interface{"eth0"};
    std::string network_test_host{"8.8.8.8"};
    std::string log_file{"/var/log/device-health-monitor.log"};
    std::vector<ServiceConfig> services;
};

bool load_config(const std::string& path, HealthConfig& config);
