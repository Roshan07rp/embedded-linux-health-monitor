cmake_minimum_required(VERSION 3.16)

project(embedded_linux_health_monitor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(health_monitor_lib
    src/cpu_monitor.cpp
    src/memory_monitor.cpp
    src/disk_monitor.cpp
    src/temperature_monitor.cpp
    src/network_monitor.cpp
    src/service_monitor.cpp
    src/logger.cpp
    src/config_manager.cpp
)

target_include_directories(health_monitor_lib PUBLIC include)

target_compile_options(health_monitor_lib PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(device-health-monitor src/main.cpp)
target_link_libraries(device-health-monitor PRIVATE health_monitor_lib)

enable_testing()

add_executable(test_cpu tests/test_cpu.cpp)
target_link_libraries(test_cpu PRIVATE health_monitor_lib)
add_test(NAME CPU_Monitor COMMAND test_cpu)

add_executable(test_memory tests/test_memory.cpp)
target_link_libraries(test_memory PRIVATE health_monitor_lib)
add_test(NAME Memory_Monitor COMMAND test_memory)

add_executable(test_config tests/test_config.cpp)
target_link_libraries(test_config PRIVATE health_monitor_lib)
add_test(NAME Config_Manager COMMAND test_config)

add_executable(test_service_monitor tests/test_service_monitor.cpp)
target_link_libraries(test_service_monitor PRIVATE health_monitor_lib)
add_test(NAME Service_Monitor COMMAND test_service_monitor)
