# Test Report

## Environment

Recommended test environment:

- Ubuntu Linux
- GCC with C++17 support
- CMake 3.16+
- systemd-enabled environment

## Build Test

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

## Unit Tests

```bash
ctest --test-dir build --output-on-failure
```

### Tests

| Test | Purpose |
|---|---|
| CPU_Monitor | Verifies CPU usage is in the 0–100% range |
| Memory_Monitor | Verifies memory usage is in the 0–100% range |
| Config_Manager | Verifies the JSON-like configuration is loaded |
| Service_Monitor | Verifies an unknown service is reported inactive |

## Manual Tests

1. Start the monitor.
2. Confirm CPU, memory and disk values are displayed.
3. Confirm network status is reported.
4. Configure a safe development service.
5. Stop the service.
6. Confirm the monitor detects the failure.
7. Confirm automatic restart when `auto_restart` is enabled.
8. Verify recovery messages in `journalctl`.

## Expected Result

The monitor should continue running after individual monitoring errors and should attempt recovery only for services explicitly configured with `auto_restart: true`.
