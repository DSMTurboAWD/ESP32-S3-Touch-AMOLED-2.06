# Waveshare ESP32-S3-Touch-AMOLED-2.06 Smart Watch & Desk Controller

ESPHome configuration for the [Waveshare ESP32-S3-Touch-AMOLED-2.06](https://www.waveshare.com/wiki/ESP32-S3-Touch-AMOLED-2.06), originally inspired by [RealDeco/xiaozhi-esphome](https://github.com/RealDeco/xiaozhi-esphome).

![watch](https://github.com/user-attachments/assets/9229cd6f-fe95-4adf-ae9c-a5d63fd10b0e)

## Configurations

- **`watchv3.yaml` (Current)**: Modular multi-screen watch & desk controller featuring:
  - **Screen 0 — Live Clock (`HH:MM:SS`)**: Real-time 1Hz clock synced across Home Assistant, SNTP, and the onboard **PCF85063** hardware RTC, with top-right **AXP2101** battery gauge, On-Air status pill, and quick Voice Assistant trigger.
  - **Screen 1 — Weather**: Vector weather condition icon, current temperature, condition label, humidity, and wind speed from Home Assistant.
  - **Screen 2 — In-Meeting / On-Air Controller**: Dedicated high-visibility toggle and live state display for `switch.on_air_screen_7b_on_air_status`.
  - **Screen 3 — Mini ESPHome Controls & Diagnostics**: 2x3 touch grid for On-Air toggle, Mic Mute, Screen Brightness presets, Speaker Volume, Voice Assistant, and Display Sleep, plus SD card capacity (`GPIO1/2/3` 1-bit SDMMC) and Reboot controls.
  - **Haptic Feedback**: Short vibration pulses via the onboard motor (`GPIO18`) on touch interactions and timer alarms.
- **`watchv1.yaml` (Rollback Baseline)**: Preserved untouched for easy rollback at any time.

## Directory Structure

```text
esphome/
├── watchv3.yaml                 # Active modular configuration
├── watchv1.yaml                 # Untouched v1 configuration for rollback
├── secrets.yaml                 # Wi-Fi credentials
├── common/
│   ├── base.yaml                # ESP32-S3, PSRAM, Wi-Fi, API, I2C, QSPI, PCF85063/HA/SNTP time
│   ├── power_imu.yaml           # AXP2101 battery gauge, QMI8658 IMU, GPIO18 haptics, brightness/sleep
│   ├── sdcard.yaml              # SD card sensors, remount button, and refresh script
│   ├── sdcard_helper.h          # Native ESP-IDF 1-bit SDMMC helper (/sdcard on GPIO1/2/3)
│   ├── audio_voice.yaml         # ES7210 ADC, ES8311 DAC, speaker, mic, mWW, Voice Assistant
│   └── touch_nav.yaml           # FT3168/FT63X6 touch polling, edge screen cycling, hit-testing
├── pages/
│   └── display.yaml             # Fonts, colors, images, and 4 watch screens + VA overlay pages
├── assets/
│   └── images/                  # Local 410x502 PNG illustrations
└── archive/
    └── watch_legacy.yaml        # Archived experimental watch.yaml
```

## Flashing

```powershell
# Compile and flash watchv3.yaml
esphome run esphome/watchv3.yaml

# Or roll back to watchv1.yaml at any time
esphome run esphome/watchv1.yaml
```
