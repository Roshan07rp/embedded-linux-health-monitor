#!/bin/bash
set -e

SERVICE="${1:-ssh}"

echo "WARNING: This will stop and restart '$SERVICE'."
echo "Use only with a non-critical development/test service."
read -r -p "Continue? [y/N] " answer

if [[ "$answer" != "y" && "$answer" != "Y" ]]; then
    echo "Cancelled."
    exit 0
fi

echo "Stopping $SERVICE..."
sudo systemctl stop "$SERVICE"

echo "Service status:"
systemctl is-active "$SERVICE" || true

echo "The health monitor should detect the failure on its next cycle."
echo "Waiting 15 seconds..."
sleep 15

echo "Starting $SERVICE again..."
sudo systemctl start "$SERVICE"

echo "Final status:"
sudo systemctl --no-pager status "$SERVICE" || true
