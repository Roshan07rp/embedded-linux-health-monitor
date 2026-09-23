# Embedded Linux Device Health Monitor & Auto-Recovery Agent

A lightweight C++17 daemon for monitoring an Embedded Linux device and automatically recovering configured services when failures are detected.

## Features

- CPU utilization monitoring from `/proc/stat`
- Memory utilization monitoring from `/proc/meminfo`
- Disk utilization monitoring using `statvfs()`
- Temperature monitoring from common Linux thermal zones
- Network interface/link and connectivity checks
- systemd service health checks with optional automatic restart
- Configurable thresholds and monitoring interval
- File and console logging
- systemd service integration
- Failure simulation script
- Lightweight unit tests without external test frameworks

## Project Structure

```text
embedded-linux-health-monitor/
├── README.md
├── CMakeLists.txt
├── .gitignore
├── config/
│   └── health_monitor.json
├── include/
│   ├── cpu_monitor.h
│   ├── memory_monitor.h
│   ├── disk_monitor.h
│   ├── temperature_monitor.h
│   ├── network_monitor.h
│   ├── service_monitor.h
│   ├── logger.h
│   └── config_manager.h
├── src/
│   ├── main.cpp
│   ├── cpu_monitor.cpp
│   ├── memory_monitor.cpp
│   ├── disk_monitor.cpp
│   ├── temperature_monitor.cpp
│   ├── network_monitor.cpp
│   ├── service_monitor.cpp
│   ├── logger.cpp
│   └── config_manager.cpp
├── systemd/
│   └── device-health-monitor.service
├── scripts/
│   ├── install.sh
│   ├── uninstall.sh
│   └── simulate_failure.sh
├── tests/
│   ├── test_cpu.cpp
│   ├── test_memory.cpp
│   ├── test_config.cpp
│   └── test_service_monitor.cpp
└── docs/
    ├── architecture.md
    └── test-report.md
```

## Requirements

Ubuntu/Debian Linux:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

The application is designed for Linux and should be tested on a real Linux system, VM, Raspberry Pi, or another embedded Linux target.

## Build

```bash
git clone https://github.com/YOUR_USERNAME/embedded-linux-health-monitor.git
cd embedded-linux-health-monitor

cmake -S . -B build
cmake --build build -j$(nproc)
```

## Run

```bash
./build/device-health-monitor --config config/health_monitor.json
```

Stop with `Ctrl+C`.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Install as a systemd service

The installation script installs the application under `/opt/device-health-monitor` and registers a systemd unit.

```bash
sudo ./scripts/install.sh
```

Check status:

```bash
sudo systemctl status device-health-monitor
```

View live logs:

```bash
sudo journalctl -u device-health-monitor -f
```

## Configuration

Edit:

```text
config/health_monitor.json
```

The default configuration checks the root filesystem, the first available thermal zone, `eth0`, and SSH. Adjust these values for your target device.

## Failure simulation

The simulation script can stop and restart a configured systemd service. Use it only on a development/test machine:

```bash
sudo ./scripts/simulate_failure.sh ssh
```

If SSH is not installed, test with another non-critical service:

```bash
sudo ./scripts/simulate_failure.sh <service-name>
```

## Architecture

See [docs/architecture.md](docs/architecture.md).

## Test Report

See [docs/test-report.md](docs/test-report.md).

## Safety Notes

Automatic service recovery executes `systemctl restart` for configured services. Only configure services that are safe to restart automatically. Do not use the failure simulation script against critical production services.

## License

MIT
