# System Architecture & Firmware Design

This document details the architectural layout, modules, pinout configuration, and data flow of the RP2040 Racing Steering Wheel firmware.

---

## 1. Firmware Directory Layout

The codebase is structured modularly to separate hardware abstraction, user interface logic, input handling, and test suites:

```text
Stering_Wheel_RP2040/
├── include/              # Global include headers
├── lib/                  # External / third-party libraries
├── src/                  # Core source code
│   ├── apps/
│   │   ├── HumanInterface/
│   │   │   ├── human_interface.h  # Base HMI controller class
│   │   │   ├── Lcd8Digit.h        # 8-digit 7-segment SPI display & menu states
│   │   │   ├── LedBar.h           # NeoPixel / WS2812B shift light controller
│   │   │   └── test.h             # HMI test hooks
│   │   └── utils/
│   │       └── timer.h            # Non-blocking timing utilities
│   ├── button_and_encoders/
│   │   ├── Encoder_KY040.h        # KY-040 rotary encoder driver
│   │   └── buttonIds.h            # Button identifier macro mappings
│   ├── defines.h                  # Hardware pin mappings and constants
│   └── main.cpp                   # Main setup, loop, interrupt handlers & GPS parser
├── test/
│   ├── unit/
│   │   └── test_all.cpp           # Unit tests for native test execution
│   └── test_all.cpp               # Consolidated test runner
├── platformio.ini                 # PlatformIO project configuration
└── TASKS.md                       # Project tracking checklist
```

---

## 2. Core Modules

### 2.1 Human Machine Interface (`src/apps/HumanInterface/`)
- **`HumanInterface` (`human_interface.h`):** Base abstract class providing common HMI state tracking (brightness, RPM, initialization and update hooks).
- **`Lcd8Digit` (`Lcd8Digit.h`):** Inherits from `HumanInterface`. Manages an 8-digit 7-segment display via SPI (`LedControl` library). Implements an interactive state machine (`tachometer`, `satelliteNumber`, `clock`, `bright`) navigable via menu rockers and encoders.
- **`LedBar` (`LedBar.h`):** Inherits from `HumanInterface`. Controls WS2812B / NeoPixel LEDs (`FastLED` / `Adafruit_NeoPixel`) for RPM progression (Green, Yellow, Red) and flash warnings at shiftlight RPM thresholds.

### 2.2 Controls & Input (`src/button_and_encoders/`)
- **`Encoder_KY040` (`Encoder_KY040.h`):** Handles quadrature decoding for dual KY-040 rotary encoders (Volume/Brightness encoder and Menu encoder) along with integrated push switch states.
- **Button Constants (`buttonIds.h`):** Defines unique integer IDs for all steering wheel inputs (Radio buttons: Green, Red, Blue, Black, Next, Back, Right, Left, Push; ECU buttons: Red, Yellow, White).

### 2.3 Utilities & Telemetry (`src/apps/utils/`)
- **`Timer` (`timer.h`):** Provides non-blocking elapsed time checks (`timePassed()`, `startTimer()`) to regulate serial transmission intervals (`MIN_INPUT_DELAY`).
- **GPS / Telemetry Parser (`src/main.cpp`):** Scans incoming UART streams for `g.` prefixed packets and extracts speed (`g.sp.`), satellite count (`g.st.`), and UTC time (`g.h.`, `g.m.`, `g.s.`).

### 2.4 Sensor Subsystem
- **BH1750 Ambient Light Sensor:** Connected via I2C (`Wire1`, SDA: `D2`, SCL: `D3`), providing real-time luminance readings for dynamic display dimming.

---

## 3. Hardware Pinout Configuration

The firmware maps to the Raspberry Pi Pico (RP2040) GPIO pins as defined in `src/defines.h`:

| Subsystem / Interface | Signal Name | GPIO Pin (Pico / RP2040) | Notes |
|-----------------------|-------------|--------------------------|-------|
| **UART0 (Wheel Serial)** | `WHEELTX` | `GPIO 0` (`D0`) | Transmission to pedalbox / telemetry hub |
|                          | `WHEELRX` | `GPIO 1` (`D1`) | Reception from telemetry hub |
| **I2C1 (Light Sensor)**  | `PIN_WIRE_SDA` / `I2C_SDA` | `GPIO 2` (`D2`) | BH1750 Light Sensor SDA |
|                          | `PIN_WIRE_SCL` / `I2C_SCL` | `GPIO 3` (`D3`) | BH1750 Light Sensor SCL |
| **Volume Encoder**       | `VOLUME_ENCODER_CW` | `GPIO 4` (`D4`) | Quadrature Clock |
|                          | `VOLUME_ENCODER_DATA` | `GPIO 5` (`D5`) | Quadrature Data |
|                          | `VOLUME_ENCODER_PRESS` | `GPIO 7` (`D7`) | Push Switch |
| **LED Strip / Shiftlights** | `LED_STRYPE_PIN` | `GPIO 6` (`D6`) | WS2812B NeoPixel Data |
| **SPI (7-Segment LCD)**  | `LCD_DIN` / `COPI` | `GPIO 11` (`D11`) | SPI Data In (COPI) |
|                          | `LCD_CLK` / `SCK` | `GPIO 10` (`D10`) | SPI Clock |
|                          | `LCD_CS` | `GPIO 9` (`D9`) | SPI Chip Select |
| **Menu Encoder**         | `MENU_ENCODER_CW` | `GPIO 13` (`D13`) | Quadrature Clock |
|                          | `MENU_ENCODER_DATA` | `GPIO 15` (`D15`) | Quadrature Data |
|                          | `MENU_ENCODER_PRESS` | `GPIO 14` (`D14`) | Push Switch |
| **ECU Buttons**          | `ECU_RED` | `GPIO 18` (`D18`) | Pull-up |
|                          | `ECU_YELLOW` | `GPIO 19` (`D19`) | Pull-up |
|                          | `ECU_WHITE` | `GPIO 17` (`D17`) | Pull-up |
| **Radio Buttons**        | `RADIO_BLACK` | `GPIO 8` (`D8`) | Pull-up |
|                          | `RADIO_BLUE` | `GPIO 22` (`D22`) | Pull-up |
|                          | `RADIO_RED` | `GPIO 23` (`D23`) | Pull-up |
|                          | `RADIO_GREEN` | `GPIO 25` (`D25`) | Pull-up |
|                          | `RADIO_BACK` | `GPIO 26` (`D26`) | Pull-up rocker / button |
|                          | `RADIO_NEXT` | `GPIO 27` (`D27`) | Pull-up rocker / button |
| **Menu Rockers**         | `MENU_UP` | `GPIO 28` (`D28`) | Pull-down |
|                          | `MENU_DOWN` | `GPIO 29` (`D29`) | Pull-down |

---

## 4. Data Flow & Execution Cycle

1. **Initialization (`setup()`):**
   - Configures serial debug output (`Serial` at 9600 baud) and wheel UART (`WHEELSERIAL` on UART0).
   - Initializes encoders, attaches hardware interrupts for rotary steps and button presses (`CHANGE`, `FALLING`, `RISING`).
   - Initializes I2C (`Wire1` at 400.7 kHz) and detects BH1750 light sensor.
   - Initializes SPI 8-digit LCD display and NeoPixel LED strip.
2. **Main Loop (`loop()`):**
   - Polls `WHEELSERIAL` for incoming telemetry strings (`g.` packets).
   - Parses GPS speed, satellite count, and UTC time.
   - Scans encoder rotations and push states, generating outbound action codes (`messageOut`).
   - Transmits batched button/encoder actions over `WHEELSERIAL` adhering to `MIN_INPUT_DELAY` debounce limits.
   - Updates HMI display states (`Lcd8Digit`) and LED bar levels.
