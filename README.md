# RP2040 Racing Steering Wheel

A dual-microcontroller racing wheel system featuring a Raspberry Pi Pico steering wheel unit and an Arduino Nano RP2040 Connect pedal box unit. The system provides GPS-based telemetry display, button controls, LED shift light indication, and digital potentiometer volume control for racing simulator applications.

## System Architecture

```
+---------------------------------------------------------------+
|                   RP2040 Racing Steering Wheel                 |
+---------------------------------------------------------------+
|                                                                |
|  +-------------------------+      +-------------------------+ |
|  |   Steering Wheel        |      |     Pedal Box           | |
|  |   (Raspberry Pi Pico)   |      | (Arduino Nano RP2040)   | |
|  |                         |      |                         | |
|  | Inputs:                |      | Inputs:                | |
|  | - 2x KY-040 Encoders   |      | - GPS Module (TinyGPS++)| |
|  | - 9x Buttons            |      |                         | |
|  | - 2x Rocker Switches    |      | Outputs:                | |
|  | - BH1750 Light Sensor   |      | - MCP4131 Digital Pot  | |
|  |                         |      |                         | |
|  | Outputs:               |<---> | Communication:          | |
|  | - MAX7219 LCD Display  | UART | - UART to GPS module   | |
|  | - NeoPixel LED Bar     | 9600 | - UART to Steering     | |
|  +-------------------------+ baud +-------------------------+ |
|                                                                |
+---------------------------------------------------------------+
```

## Current Status

### Working Features

**Steering Wheel (Raspberry Pi Pico)**
- 2x KY-040 rotary encoders with quadrature decoding (volume and menu control)
- 9x push buttons with internal pull-ups and debouncing (6 radio, 3 ECU)
- 2x rocker switches for menu navigation
- MAX7219 8-digit 7-segment display via LedControl library
- BH1750 I2C light sensor for automatic brightness adjustment
- UART1 communication to Pedal Box at 960 baud

**Pedal Box (Arduino Nano RP2040 Connect)**
- GPS module with TinyGPS++ parsing (NMEA sentences)
- Extracts: time, speed, position, heading, satellite count
- MCP4131 digital potentiometer via SPI for radio volume control
- GPS data forwarding to steering wheel via UART0

### Known Issues

**BUG-001 (High Priority)**: GPS data received on Steering Wheel is not parsed. The Pedal Box correctly sends `g.sp.45 g.st.8 g.h.14 g.m.30 g.s.25` but the Steering Wheel reads the message without extracting values. Speed, satellite count, and time are not displayed on the LCD.

**BUG-002 (Medium Priority)**: Persistent storage is not implemented. StorageManager class is referenced but commented out. All user settings (volume, timezone, brightness) are lost on power cycle.

## Directory Structure

```
RP2040_Racing_Steering_Wheel/
├── Steering_Wheel/
│   └── Stering_Wheel_RP2040/
│       ├── platformio.ini          # PlatformIO config (Pico)
│       └── src/
│           ├── main.cpp             # Main steering wheel logic
│           ├── defines.h            # Pin definitions
│           ├── apps/
│           │   └── HumanInterface/  # LCD, LED, encoder classes
│           └── button_and_encoders/ # Rotary encoder handling
├── Pedal_Box/
│   └── Pedal_Box_RP2040/
│       ├── platformio.ini          # PlatformIO config (Nano RP2040)
│       └── src/
│           ├── main.cpp             # Main pedal box logic
│           ├── defines.h            # Pin definitions
│           └── apps/utils/          # Utility functions
├── docs/
│   ├── architecture.md              # Hardware architecture & pinout
│   ├── features.md                  # Feature list, bugs, specifications
│   ├── communications.md            # UART protocol documentation
│   └── testing.md                   # Testing procedures
└── README.md                        # This file
```

## Getting Started

### Prerequisites

- PlatformIO Core or PlatformIO IDE
- 2x micro-USB cables (one for each board)
- 4-wire connection between boards (TX, RX, VCC, GND)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/your-repo/RP2040_Racing_Steering_Wheel.git
cd RP2040_Racing_Steering_Wheel

# Build Steering Wheel (Pico)
cd Steering_Wheel/Stering_Wheel_RP2040
pio run

# Build Pedal Box (Nano RP2040)
cd Pedal_Box/Pedal_Box_RP2040
pio run
```

### Upload

```bash
# Upload to Steering Wheel
cd Steering_Wheel/Stering_Wheel_RP2040
pio run --target upload

# Upload to Pedal Box
cd Pedal_Box/Pedal_Box_RP2040
pio run --target upload
```

## Testing

### Hardware Verification

1. **Steering Wheel**: Open serial monitor at 9600 baud - button presses should log to console
2. **Pedal Box**: Verify GPS module acquisition - serial output shows satellite count
3. **Inter-board**: Monitor both serial outputs to confirm UART message exchange

### Functional Tests

- [ ] All buttons register correctly with debounced input
- [ ] Encoders increment/decrement properly
- [ ] LCD displays speed, time, satellite count, heading
- [ ] GPS data parses correctly from Pedal Box
- [ ] Volume control adjusts MCP4131 resistance
- [ ] Light sensor reading functional

## Contributing

Contributions are welcome. Please follow these guidelines:

1. Create a feature branch for new work
2. Reference existing bugs (BUG-001, BUG-002) when addressing them
3. Update relevant documentation in `docs/` when changing functionality
4. Test thoroughly before submitting pull requests

For detailed feature specifications and implementation plans, see `docs/features.md`.

---

**Last Updated**: April 2026