#include "network_monitor.h"
#include <fstream>
#include <cstdlib>
#include <string>

bool is_network_interface_up(const char* interface_name) {
    const std::string path = std::string("/sys/class/net/") + interface_name + "/operstate";
    std::ifstream file(path);
    std::string state;
    return static_cast<bool>(file >> state) && (state == "up" || state == "unknown");
}

bool can_reach_host(const char* host) {
    const std::string command =
        "ping -c 1 -W 1 " + std::string(host) + " > /dev/null 2>&1";
    return std::system(command.c_str()) == 0;
}
