#include "service_monitor.h"
#include <cstdlib>

bool is_service_active(const std::string& service) {
    const std::string command =
        "systemctl is-active --quiet " + service + " > /dev/null 2>&1";
    return std::system(command.c_str()) == 0;
}

bool restart_service(const std::string& service) {
    const std::string command =
        "systemctl restart " + service + " > /dev/null 2>&1";
    return std::system(command.c_str()) == 0;
}
