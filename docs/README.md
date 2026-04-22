# RP2040 Racing Steering Wheel Project

A custom racing wheel controller built with Raspberry Pi Pico and Arduino Nano RP2040 Connect platforms, designed for sim racing and automotive applications.

## Project Overview

This project implements a dual-board racing wheel system featuring:

- **Steering Wheel Unit** (Raspberry Pi Pico): Handles button inputs, rotary encoders, LCD display, and shift light LED bar
- **Pedal Box Unit** (Arduino Nano RP2040 Connect): Manages GPS data, digital potentiometer control, and communication relay

## Objectives

- Create a functional racing wheel controller with realistic button layout
- Implement GPS-based telemetry display
- Enable inter-board communication for coordinated operation
- Provide a foundation for future expansion (RPM input, RTC, persistent storage)

## Directory Structure

```
RP2040_Racing_Steering_Wheel/
├── Steering_Wheel/
│   └── Stering_Wheel_RP2040/          # PlatformIO project for Pico
│       ├── src/
│       │   └── main.cpp
│       ├── include/
│       └── platformio.ini
├── Pedal_Box/
│   └── Pedal_Box_RP2040/              # PlatformIO project for Nano RP2040
│       ├── src/
│       │   └── main.cpp
│       ├── include/
│       └── platformio.ini
├── docs/                               # This documentation
└── LICENSE
```

## Hardware Requirements

### Steering Wheel Board
- Raspberry Pi Pico
- 2x KY-040 rotary encoders
- 9x push buttons (various colors)
- 2x rocker switches
- 8-digit 7-segment LCD display (MAX7219)
- 8-LED NeoPixel bar
- BH1750 light sensor (I2C)

### Pedal Box Board
- Arduino Nano RP2040 Connect
- GPS module (TinyGPS++ compatible)
- MCP4131 digital potentiometer (SPI)

## Debug Configuration for Raspberry Pi Pico

### Current State
The project uses UART0 (Serial1) for inter-board communication. Serial output goes to USB by default with Arduino-Pico core.

### Enable USB Debug Output

The Pico's `Serial` object is USB CDC by default. To enable debug output:

1. **platformio.ini** - Add these settings:
```ini
[env:pico]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool

; Debug settings
debug_tool = picoprobe
debug_build_flags =
    -O2
    -g
    -ggdb
monitor_speed = 115200
```

2. **In code** - Use `Serial.begin(115200)` (baud rate ignored for USB but needed for compatibility)

### Debugging Options

#### Option 1: PicoProbe (Recommended - uses another Pico)
- Flash picoprobe UF2 on second Pico
- Connect: SWDIO → GP2, SWCLK → GP3, GND → GND
- platformio.ini:
```ini
upload_protocol = picoprobe
debug_tool = picoprobe
```

#### Option 2: CMSIS-DAP (J-Link or official Debug Probe)
```ini
upload_protocol = cmsis-dap
debug_tool = cmsis-dap
```

#### Option 3: No Hardware Debug (USB Serial Only)
- Use Serial Monitor via USB
- No special platformio.ini changes needed
- Just add `Serial.println("debug message")` in code

### Serial Reference Table

| Object | Hardware | Default Pins | Use Case |
|--------|----------|--------------|----------|
| Serial | USB CDC | - | Debug output to PC |
| Serial1 | UART0 | GP0=TX, GP1=RX | Pedal Box communication |
| Serial2 | UART1 | GP4=TX, GP5=RX | Not used |

### Notes
- With debug probe, `Serial` goes to USB, UART is separate
- If using PicoProbe, USB is used for debug, so Serial Monitor shows output
- Current project uses Serial1 (UART0) for Pedal Box - this is correct
- To add USB debug, just use `Serial.print()` statements in code

## Quick Start

### Building the Projects

1. Install PlatformIO:
   ```bash
   # Via Homebrew (macOS)
   brew install platformio
   
   # Or via pip
   pip install platformio
   ```

2. Build and upload to Steering Wheel:
   ```bash
   cd Steering_Wheel/Stering_Wheel_RP2040
   pio run --target upload
   ```

3. Build and upload to Pedal Box:
   ```bash
   cd Pedal_Box/Pedal_Box_RP2040
   pio run --target upload
   ```

### Wiring

See [Architecture Documentation](architecture.md) for complete pinout details.

## Communication

The two boards communicate via UART at 9600 baud. See [Communications Documentation](communications.md) for protocol details.

## Current Status

This project is under active development. See [Features Documentation](features.md) for implemented features and planned improvements.

## License

See LICENSE file for details.