#include "memory_monitor.h"
#include <cassert>
#include <iostream>

int main() {
    const double usage = get_memory_usage();
    assert(usage >= 0.0 && usage <= 100.0);
    std::cout << "Memory test passed: " << usage << "%\n";
    return 0;
}
