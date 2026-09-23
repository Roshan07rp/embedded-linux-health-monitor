#include "disk_monitor.h"
#include <sys/statvfs.h>

double get_disk_usage(const char* path) {
    struct statvfs fs{};
    if (statvfs(path, &fs) != 0 || fs.f_blocks == 0) return -1.0;

    const double total = static_cast<double>(fs.f_blocks);
    const double free_blocks = static_cast<double>(fs.f_bfree);
    return 100.0 * (1.0 - free_blocks / total);
}
