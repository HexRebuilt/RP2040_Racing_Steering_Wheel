# Setup & Hardware Guide

This guide covers development environment prerequisites, hardware wiring instructions, build and flashing procedures, and test suite execution for the RP2040 Racing Steering Wheel.

---

## 1. Prerequisites

- **Visual Studio Code:** Recommended IDE for PlatformIO development.
- **PlatformIO IDE Extension:** Installed within VS Code.
- **Python 3.x:** Required by PlatformIO core and build scripts.
- **USB Cable:** Micro-USB / USB-C cable to connect the Raspberry Pi Pico to your development machine.

---

## 2. Environment & PlatformIO Configuration

The project uses PlatformIO (`platformio.ini`) with three distinct environments:

1. **`[env:pico]`:** Production build targeting the Raspberry Pi Pico (`earlephilhower` Arduino core, `picotool` upload protocol).
2. **`[env:test]`:** Target build configuration with test source inclusion (`-D UNIT_TEST`).
3. **`[env:native_test]`:** Host-native test environment using Unity test framework and `ArduinoNative` library (`src_filter` excludes `main.cpp`).

---

## 3. Hardware Wiring & Pinout Connection

Ensure your steering wheel electronics are wired according to the pinout table specified in [Architecture Documentation (`docs/ARCHITECTURE.md#3-hardware-pinout-configuration`)](ARCHITECTURE.md#3-hardware-pinout-configuration).

- **Power Supply:** Ensure clean 3.3V / 5V rails for the RP2040, SPI display, and NeoPixel LED bar (note: keep LED brightness limits `MAX_BRIGHT_LEDS` configured to prevent USB power overload).
- **Encoders:** Connect KY-040 CW, DT (Data), and SW (Switch) pins to the designated GPIO pins with pull-ups as required.
- **I2C Light Sensor:** Connect BH1750 SDA to `GPIO 2` and SCL to `GPIO 3`.

---

## 4. Build, Compilation, and Flashing

### 4.1 Building the Firmware
To compile the firmware for the Raspberry Pi Pico:
```bash
pio run -e pico
```

### 4.2 Flashing to RP2040
1. Put the Raspberry Pi Pico into **BOOTSEL mode** (hold the BOOTSEL button while connecting the USB cable to your computer).
2. Upload the compiled firmware using PlatformIO:
   ```bash
   pio run -e pico --target upload
   ```
   *PlatformIO will automatically detect the RP2040 bootloader via `picotool`.*

---

## 5. Running the Test Suites

### 5.1 Running Native Unit Tests (Host Machine)
You can run the unit test suite on your host machine without requiring physical hardware connected:
```bash
pio test -e native_test
```
This executes tests covering button ID constants, GPS string parsing algorithms, and timer logic using the Unity test framework.

### 5.2 Target Hardware Testing
To run tests on connected hardware target:
```bash
pio test -e test
```
