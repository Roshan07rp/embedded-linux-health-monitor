# Architecture

## Overview

The Device Health Monitor is a Linux daemon written in C++17.

```text
                         +----------------------+
                         |       main.cpp       |
                         +----------+-----------+
                                    |
             +----------------------+----------------------+
             |          |            |          |          |
             v          v            v          v          v
           CPU       Memory         Disk    Temperature  Network
         Monitor     Monitor       Monitor     Monitor    Monitor
             |          |            |          |          |
             +----------+------------+----------+----------+
                                    |
                                    v
                            Service Monitor
                                    |
                                    v
                              Logger/Recovery
                                    |
                                    v
                              systemd / Linux
```

## Data Sources

- CPU: `/proc/stat`
- Memory: `/proc/meminfo`
- Disk: `statvfs()`
- Temperature: `/sys/class/thermal/*/temp`
- Network interface: `/sys/class/net/<interface>/operstate`
- Network reachability: `ping`
- Service state/recovery: `systemctl`

## Recovery Flow

1. Monitor configured services.
2. Detect an inactive service.
3. Write an error to the log.
4. If `auto_restart` is enabled, execute `systemctl restart`.
5. Record success or failure.

## Design Considerations

The monitor is intentionally modular so that individual sensors can be replaced for a specific embedded board. Hardware-specific temperature paths and network interfaces vary between Linux targets, so these are configuration-dependent.
