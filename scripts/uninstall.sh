#!/bin/bash
set -e

INSTALL_DIR="/opt/device-health-monitor"

sudo systemctl disable --now device-health-monitor 2>/dev/null || true
sudo rm -f /etc/systemd/system/device-health-monitor.service
sudo systemctl daemon-reload
sudo rm -rf "$INSTALL_DIR"

echo "Device Health Monitor removed."
