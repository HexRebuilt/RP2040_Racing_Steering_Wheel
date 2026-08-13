# RP2040 Racing Steering Wheel Firmware

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-blue.svg)](https://platformio.org/)
[![RP2040](https://img.shields.io/badge/MCU-Raspberry%20Pi%20Pico%20(RP2040)-green.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg)](LICENSE)

Firmware and telemetry interface for the custom **RP2040 Racing Steering Wheel**, designed for motorsport simulation and real-time telemetry display. Powered by the Raspberry Pi Pico (RP2040 microcontroller), this firmware manages high-frequency input controls, LED shift lights, an 8-digit 7-segment SPI display, ambient light sensing, and bidirectional UART telemetry communication.

---

## Key Features

- **Human Machine Interface (HMI):**
  - **8-Digit 7-Segment Display (`Lcd8Digit`):** SPI-driven display supporting multiple operational modes including Tachometer, Speed, GPS Satellites count, Clock, and Brightness adjustment.
  - **NeoPixel / WS2812B LED Bar (`LedBar`):** Dynamic shift light indicator with color-coded RPM zones (Green, Yellow, Red, Shiftlight flash).
- **Controls & Input Subsystem:**
  - Dual **KY-040 Rotary Encoders** with push-button switches for volume, menu navigation, and parameter adjustments.
  - Comprehensive steering wheel button matrix including radio buttons (Green, Red, Blue, Black, Next, Back) and ECU toggle buttons (Red, Yellow, White).
  - Rocker switches for menu navigation (Up, Down).
- **Sensors & Telemetry:**
  - **BH1750 Ambient Light Sensor** over I2C (`Wire1`) for automatic display/LED brightness adaptation.
  - Robust **GPS & Telemetry Parser** over UART (`WHEELSERIAL` / UART0) handling real-time data packets (`g.sp.`, `g.st.`, `g.h.`, `g.m.`, `g.s.`).
- **Testing & Verification:**
  - Comprehensive test suite supporting native host execution (Unity test framework) and target hardware validation.

---

## Hardware Requirements

- **Microcontroller:** Raspberry Pi Pico / RP2040 Development Board.
- **Display:** 8-digit 7-segment LED display via SPI (`MAX7219` or compatible driver).
- **LEDs:** WS2812B Addressable LED strip / NeoPixel bar.
- **Encoders:** 2x KY-040 Rotary Encoders with integrated push switches.
- **Light Sensor:** BH1750 I2C Digital Ambient Light Sensor.
- **Inputs:** Push buttons and rocker switches with pull-up/pull-down configuration.

---

## Quick Start Guide

1. **Prerequisites:** Install [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO IDE](https://platformio.org/) extension.
2. **Clone / Open Repository:** Open this repository root directory in VS Code / PlatformIO.
3. **Build Firmware:**
   ```bash
   pio run -e pico
   ```
4. **Flash to RP2040:** Connect the Raspberry Pi Pico in BOOTSEL mode and upload:
   ```bash
   pio run -e pico --target upload
   ```

---

## Documentation

For deeper technical details, explore the documentation files in the `docs/` directory:
- [Architecture & Modules (`docs/ARCHITECTURE.md`)](docs/ARCHITECTURE.md)
- [Setup & Hardware Guide (`docs/SETUP.md`)](docs/SETUP.md)
- [Telemetry & Protocol Specification (`docs/PROTOCOL.md`)](docs/PROTOCOL.md)

---

## Contributing & License

Contributions are welcome! Please ensure code style consistency and run the test suite (`pio test -e native_test`) before submitting pull requests.

Distributed under the MIT License. See `LICENSE` for details.
