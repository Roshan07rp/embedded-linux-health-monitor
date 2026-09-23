#include "service_monitor.h"
#include <cassert>
#include <iostream>

int main() {
    // systemd itself is expected to exist on the target Linux system.
    const bool result = is_service_active("this-service-should-not-exist");
    assert(!result);
    std::cout << "Service monitor test passed.\n";
    return 0;
}
