#include "config_manager.h"
#include <cassert>
#include <iostream>

int main() {
    HealthConfig config;
    const bool loaded = load_config("../config/health_monitor.json", config);
    assert(loaded);
    assert(config.interval_seconds > 0);
    assert(!config.disk_path.empty());
    assert(!config.network_interface.empty());
    assert(!config.services.empty());
    std::cout << "Config test passed.\n";
    return 0;
}
