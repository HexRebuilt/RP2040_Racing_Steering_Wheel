# Pinout Mapping Documentation

/* Taken from https://github.com/arduino/ArduinoCore-mbed/tree/master/variants/NANO_RP2040_CONNECT */

This document maps the **D** (display/IO) pin numbers used in the firmware to the **GPIO** (GP) pins on the RP2040 microcontroller. The physical connections are shown in the board's pinout image:

![Pinout Image](/docs/`waveshare_mini_rp2040_ pinout.jpg`)

> **Note**: The image shows the actual hardware layout. The table below translates the logical D numbers used in the code to the corresponding GP pins (with alternative `pXY` naming from the ArduinoCore‑mbed variant).

## D‑to‑GP Mapping Table

| D # | GP Pin |
|-----|--------|
| D0  | GP0 |
| D1  | GP1 |
| D2  | GP# |
| D3  | GP15 |
| D4 | *Not assigned* |
| D5  | GP17 |
| D6  | GP18 |
| D7  | GP19 |
| D8  | GP20 |
| D9  | GP21 |
| D10 | GP5 |
| D11 | GP7 |
| D12 | GP4 |
| D13 | GP6 |
| D14  | GP26 |
| D15  | GP27 |
| D16  | GP28 |
| D17  | GP29 |
| D18  | GP12 |
| D19  | GP13 |
| D20  | GP2 |
| D21  | GP# |
| D22 | *Not assigned* |
| D23 | *Not assigned* |
| D24  | GP3 |
| D25  | GP8 |
| D26  | GP9 |
| D27  | GP10 |
| D28  | GP11 |
| D29  | GP14 |

## How to Use in PlatformIO

1. **Include the mapping header** in your source files (e.g., `pinout.h`):
   ```c
   #define D0 GP0
   #define D1 GP1
   // ... continue for all D numbers used
   ```

2. **Configure the board** in `platformio.ini` to use the correct environment:
   ```ini
   [env:nanorp2040connect]
   platform = raspberrypi
   board = nanorp2040connect
   framework = arduino
   ```

3. **Set pin modes** in your `setup()` function:
   ```cpp
   void setup() {
       pinMode(D0, INPUT_PULLUP);
       pinMode(D1, INPUT_PULLUP);
       // ... configure all required pins
   }
   ```

4. **Refer to the image** for physical connector positions if you need to wire the board.

## Verification

- Ensure all assigned pins are correctly connected to the hardware as shown in the pinout image.
- Run `platformio run` and check that the firmware compiles without errors related to undefined pin names.
- Use `gpio readall` (or equivalent) in your test harness to verify that the expected pins reflect the logical D numbers.

---

*Generated on 2026‑08‑14. For any questions, refer to the hardware schematic (`sheet3_digital_inputs.sch`) or contact the hardware team.*