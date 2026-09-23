#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
INSTALL_DIR="/opt/device-health-monitor"

echo "[1/5] Building project..."
cmake -S "$PROJECT_DIR" -B "$PROJECT_DIR/build"
cmake --build "$PROJECT_DIR/build" -j"$(nproc)"

echo "[2/5] Installing files..."
sudo mkdir -p "$INSTALL_DIR/bin" "$INSTALL_DIR/config"
sudo cp "$PROJECT_DIR/build/device-health-monitor" "$INSTALL_DIR/bin/"
sudo cp "$PROJECT_DIR/config/health_monitor.json" "$INSTALL_DIR/config/"

echo "[3/5] Installing systemd unit..."
sudo cp "$PROJECT_DIR/systemd/device-health-monitor.service" /etc/systemd/system/

echo "[4/5] Reloading systemd..."
sudo systemctl daemon-reload
sudo systemctl enable device-health-monitor

echo "[5/5] Starting service..."
sudo systemctl restart device-health-monitor

echo
echo "Installation complete."
sudo systemctl --no-pager status device-health-monitor
