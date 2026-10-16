#!/usr/bin/env bash
set -euo pipefail
ESP32_CORE="3.3.12"
INDEX="https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"
if [[ "$ESP32_CORE" == *x* ]]; then echo "Renseigne ESP32_CORE dans $0"; exit 1; fi
arduino-cli core update-index --additional-urls "$INDEX"
arduino-cli core install "esp32:esp32@${ESP32_CORE}" --additional-urls "$INDEX"
arduino-cli lib install "U8g2@2.36.19"
