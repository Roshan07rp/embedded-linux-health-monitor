#include "cpu_monitor.h"
#include <cassert>
#include <iostream>

int main() {
    const double usage = get_cpu_usage();
    assert(usage >= 0.0 && usage <= 100.0);
    std::cout << "CPU test passed: " << usage << "%\n";
    return 0;
}
