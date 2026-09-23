#include "config_manager.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "temperature_monitor.h"
#include "network_monitor.h"
#include "service_monitor.h"
#include "logger.h"

#include <chrono>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <thread>

namespace {
volatile std::sig_atomic_t running = 1;

void signal_handler(int) {
    running = 0;
}

void report_threshold(Logger& logger, const std::string& name, double value,
                      double warning, double critical) {
    if (value < 0) return;

    if (value >= critical) {
        logger.log(LogLevel::ERROR,
                   name + " CRITICAL: " + std::to_string(value));
    } else if (value >= warning) {
        logger.log(LogLevel::WARNING,
                   name + " WARNING: " + std::to_string(value));
    }
}
}

int main(int argc, char* argv[]) {
    std::string config_path = "config/health_monitor.json";
    if (argc >= 3 && std::string(argv[1]) == "--config")
        config_path = argv[2];

    HealthConfig config;
    if (!load_config(config_path, config)) {
        std::cerr << "Failed to load configuration: " << config_path << '\n';
        return 1;
    }

    Logger logger(config.log_file);
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    logger.log(LogLevel::INFO, "Device Health Monitor started.");

    while (running) {
        const double cpu = get_cpu_usage();
        const double memory = get_memory_usage();
        const double disk = get_disk_usage(config.disk_path.c_str());
        const double temperature = get_temperature_celsius();

        const bool interface_up =
            is_network_interface_up(config.network_interface.c_str());
        const bool internet_up =
            can_reach_host(config.network_test_host.c_str());

        std::cout << "\n========== Device Health ==========\n";
        std::cout << std::fixed << std::setprecision(1);
        if (cpu >= 0) std::cout << "CPU Usage       : " << cpu << " %\n";
        if (memory >= 0) std::cout << "Memory Usage    : " << memory << " %\n";
        if (disk >= 0) std::cout << "Disk Usage      : " << disk << " %\n";
        if (temperature >= 0) std::cout << "Temperature     : " << temperature << " C\n";
        std::cout << "Network Interface: "
                  << (interface_up ? "UP" : "DOWN") << '\n';
        std::cout << "Network Reach   : "
                  << (internet_up ? "ONLINE" : "OFFLINE") << '\n';

        report_threshold(logger, "CPU", cpu,
                         config.cpu_warning_percent, config.cpu_critical_percent);
        report_threshold(logger, "Memory", memory,
                         config.memory_warning_percent, config.memory_critical_percent);
        report_threshold(logger, "Disk", disk,
                         config.disk_warning_percent, config.disk_critical_percent);
        report_threshold(logger, "Temperature", temperature,
                         config.temperature_warning_celsius,
                         config.temperature_critical_celsius);

        for (const auto& service : config.services) {
            const bool active = is_service_active(service.name);
            std::cout << service.name << " Service   : "
                      << (active ? "RUNNING" : "STOPPED") << '\n';

            if (!active) {
                logger.log(LogLevel::ERROR,
                           service.name + " is inactive.");

                if (service.auto_restart) {
                    logger.log(LogLevel::WARNING,
                               "Attempting automatic recovery for " + service.name);

                    if (restart_service(service.name)) {
                        logger.log(LogLevel::INFO,
                                   service.name + " restarted successfully.");
                    } else {
                        logger.log(LogLevel::ERROR,
                                   "Recovery failed for " + service.name);
                    }
                }
            }
        }

        std::cout << "===================================\n";
        std::this_thread::sleep_for(
            std::chrono::seconds(config.interval_seconds));
    }

    logger.log(LogLevel::INFO, "Device Health Monitor stopped.");
    return 0;
}
