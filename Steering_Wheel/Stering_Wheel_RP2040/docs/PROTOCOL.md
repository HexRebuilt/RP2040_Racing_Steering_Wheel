# Telemetry & Communication Protocol Specification

This document defines the serial communication parameters, inbound telemetry message formats, outbound control packet structures, and button identifier mappings for the RP2040 Racing Steering Wheel.

---

## 1. Serial Communication Parameters

- **Interface:** UART0 (`Serial1`) on the Raspberry Pi Pico.
- **Baud Rate:** `9600` bps.
- **Data Bits:** 8
- **Parity:** None
- **Stop Bits:** 1
- **Line Ending:** Newline (`\n`) terminated strings.

---

## 2. Inbound Telemetry Protocol (Telemetry Hub / GPS to Wheel)

The steering wheel receives telemetry and GPS updates from the vehicle data hub over `WHEELSERIAL`. Messages must start with the `"g."` prefix.

### Expected Message Format
```text
g.sp.X g.st.X g.h.X g.m.X g.s.X\n
```

### Key-Value Parameters
| Key Prefix | Parameter | Description | Data Type | Example |
|------------|-----------|-------------|-----------|---------|
| `g.sp.` | Speed | Vehicle speed (e.g. km/h or mph) | Float / Int | `g.sp.45` |
| `g.st.` | Satellites | Number of connected GPS satellites | Integer | `g.st.8` |
| `g.h.` | Hours | UTC time - Hours | Integer | `g.h.14` |
| `g.m.` | Minutes | UTC time - Minutes | Integer | `g.m.30` |
| `g.s.` | Seconds | UTC time - Seconds | Integer | `g.s.25` |

### Parsing Implementation Reference (`src/main.cpp`)
The firmware scans incoming strings for each key and extracts values using substring and conversion routines:
```cpp
if (messageIn.startsWith("g.")) {
  parseGPSData(messageIn);
}
```

---

## 3. Outbound Control Protocol (Wheel to Pedalbox / ECU)

When the driver interacts with steering wheel buttons, rotary encoders, or rockers, the steering wheel transmits control action strings terminated by `\n`.

### Debounce & Transmission Delay
- To prevent serial flooding and switch bouncing, outbound transmissions are throttled by `MIN_INPUT_DELAY` (default `175 ms`).

### Button & Action Identifier Macros (`buttonIds.h`)

| Macro Name | ID Value | Description | Trigger Source |
|------------|----------|-------------|----------------|
| `RGREEN` | `1` | Radio Green Button | Button press (Falling) |
| `RRED` | `2` | Radio Red Button | Button press (Falling) |
| `RBLUE` | `3` | Radio Blue Button | Button press (Falling) |
| `RBLACK` | `4` | Radio Black Button | Button press (Falling) |
| `RUP` | `5` | Radio Next Rocker | Rocker switch (Rising) |
| `RDOWN` | `6` | Radio Back Rocker | Rocker switch (Rising) |
| `RRIGHT` | `7` | Volume Encoder Right (Clockwise) | Encoder rotation |
| `RLEFT` | `8` | Volume Encoder Left (Counter-Clockwise) | Encoder rotation |
| `RPUSH` | `9` | Menu Encoder Press / Volume Press | Encoder push switch |
| `LRED` | `10` | ECU Red Button | Button press (Falling) |
| `LYELLOW` | `11` | ECU Yellow Button | Button press (Falling) |
| `LWHITE` | `12` | ECU White Button | Button press (Falling) |

---

## 4. Message Transmission Example

When the driver turns the volume encoder clockwise:
1. `volumeEncoder.Steps()` detects positive delta.
2. `messageOut` is assigned `String(RRIGHT)` (`"7"`).
3. The main loop formats the message: `"7\n"`.
4. Transmitted over `WHEELSERIAL` (UART0).
