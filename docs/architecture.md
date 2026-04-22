# System Architecture

This document describes the hardware architecture, pin assignments, and inter-board connections for the RP2040 Racing Steering Wheel system.

## Overview

The system consists of two independent PlatformIO projects running on separate microcontrollers:

1. **Steering Wheel Unit**: Raspberry Pi Pico (Arduino framework)
2. **Pedal Box Unit**: Arduino Nano RP2040 Connect

Both units communicate via UART to coordinate functionality and share data.

## Steering Wheel (Raspberry Pi Pico)

### Pinout Assignment

| Function | GPIO Pin | Physical Pin | Notes |
|----------|----------|---------------|-------|
| UART1 TX | GP0 | Pin 1 | To Pedal Box RX |
| UART1 RX | GP1 | Pin 2 | From Pedal Box TX |
| I2C SDA | GP2 | Pin 4 | BH1750 Light Sensor |
| I2C SCL | GP3 | Pin 5 | BH1750 Light Sensor |
| Encoder 1 Clock | GP4 | Pin 6 | KY-040 rotary encoder |
| Encoder 1 Data | GP5 | Pin 7 | KY-040 rotary encoder |
| Encoder 1 Switch | GP7 | Pin 10 | KY-040 push button |
| Button (Black) | GP8 | Pin 11 | Radio buttons |
| LCD DIN | GP11 | Pin 14 | MAX7219 display |
| LCD CLK | GP10 | Pin 15 | MAX7219 display |
| LCD CS | GP9 | Pin 16 | MAX7219 display |
| Encoder 2 Clock | GP13 | Pin 17 | KY-040 rotary encoder |
| Encoder 2 Data | GP15 | Pin 20 | KY-040 rotary encoder |
| Encoder 2 Switch | GP14 | Pin 19 | KY-040 push button |
| Button (White) | GP17 | Pin 22 | ECU button |
| Button (Red) | GP18 | Pin 23 | ECU button |
| Button (Yellow) | GP19 | Pin 24 | ECU button |
| Button (Blue) | GP22 | Pin 29 | Radio buttons |
| Button (Red) | GP23 | Pin 30 | Radio buttons |
| Button (Green) | GP25 | Pin 31 | Radio buttons |
| Button (Back) | GP26 | Pin 32 | Radio buttons |
| Button (Next) | GP27 | Pin 33 | Radio buttons |
| Rocker Up | GP28 | Pin 34 | Menu navigation |
| Rocker Down | GP29 | Pin 35 | Menu navigation |

### Connected Peripherals

#### 2x KY-040 Rotary Encoders
- **Encoder 1**: Volume control (clock=GP4, data=GP5, switch=GP7)
- **Encoder 2**: Menu navigation (clock=GP13, data=GP15, switch=GP14)

#### Button Matrix (9 buttons)
- **Radio Control Buttons**:
  - Red (GP23)
  - Blue (GP22)
  - Green (GP25)
  - Black (GP8)
  - Next (GP27)
  - Back (GP26)
- **ECU Buttons**:
  - Red (GP18)
  - Yellow (GP19)
  - White (GP17)

#### Rocker Switches
- **Menu Navigation**:
  - Up (GP28)
  - Down (GP29)

#### Display (MAX7219 via LedControl)
- 8-digit 7-segment display
- SPI interface (DIN=GP11, CLK=GP10, CS=GP9)

#### LED Bar (NeoPixel)
- 8 LEDs for shift light indication
- Data line connected (pin to be verified in code)

#### Light Sensor (BH1750)
- I2C interface (SDA=GP2, SCL=GP3)
- Used for automatic display brightness adjustment

## Pedal Box (Arduino Nano RP2040 Connect)

### Pinout Assignment

| Function | Pin | Notes |
|----------|-----|-------|
| UART0 TX | p0 | To Steering Wheel RX |
| UART0 RX | p1 | From Steering Wheel TX |
| GPS TX | p8 | From GPS module |
| GPS RX | p9 | To GPS module |
| SPI COPI | p11 | To MCP4131 |
| SPI CIPO | p12 | From MCP4131 |
| SPI SCK | p10 | Clock to MCP4131 |
| MCP4131 CS | p2 | Chip select |

### Connected Peripherals

#### GPS Module (TinyGPS++)
- UART interface (TX=p8, RX=p9)
- Parses NMEA sentences for time, speed, and position data

#### Digital Potentiometer (MCP4131)
- SPI interface for radio volume control
- 100kΩ potentiometer (verify in code)
- Chip select on p2

## Inter-Board Communication

### Connection Diagram

```
Steering Wheel (Pico)              Pedal Box (Nano RP2040)
┌─────────────────────┐            ┌─────────────────────┐
│     UART1          │            │     UART0          │
│  GP0 (TX) ────────►│───────────►│ p1 (RX)            │
│  GP1 (RX) ◄────────│───────────◄│ p0 (TX)            │
└─────────────────────┘            └─────────────────────┘
```

### Communication Parameters
- **Baud Rate**: 9600
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None

## Power Supply

- Both boards powered via USB (5V)
- Pico: USB micro-B connector
- Nano RP2040: USB-C connector
- Current estimate: ~500mA total (including NeoPixels at full brightness)

## Physical Considerations

- Steering wheel mounted in custom 3D-printed enclosure
- Pedal box located in separate housing near pedals
- Cable routing: 4-wire connection between units (TX, RX, VCC, GND)
- Recommended cable: 4-conductor dupont/jumper wire or Ethernet cable

## Expansion Headers

### Pico (Unassigned GPIO)
- GP6, GP12, GP16, GP20, GP21 - Available for future expansion

### Nano RP2040
- p3-p7, p13-p20 - Available pins (verify not used by WiFi/BT)

## Notes

- RPM input pin is defined in code but not currently connected
- Some pins have pull-up/pull-down resistors configured in software
- I2C and SPI share some pins - verify no conflicts when adding peripherals