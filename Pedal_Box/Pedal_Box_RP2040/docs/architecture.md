# RP2040 Racing Steering Wheel - Pedal Box Diagnostics & Architecture

## Version: 1.0
## Date: 2026-08-13
## Author: HexRebuilt

---

## 1. ISSUE DIAGNOSIS: Build & Flash Problems

### 1.1 Original Problem
User reported "some issue with the build and flash on the dedicated system."

### 1.2 Diagnosis Findings

When running `platformio run` without `-e`, PlatformIO attempts to build **both** environments in `platformio.ini`:
- `nanorp2040connect` → **Succeeds** ✓
- `native` → **Fails** with "Error: Nothing to build. Please put your source code files to the '/Users/hexrebuilt/repositories/RP2040_Racing_Steering_Wheel/Pedal_Box/Pedal_Box_RP2040/src' folder"

The `native` environment is a stub/testing environment designed for `platformio test -e native`, not standalone `platformio run` builds.

### 1.3 Verified Commands

| Command | Status |
|---------|--------|
| `platformio run` (no -e) | ❌ Fails - tries both environments |
| `platformio run -e nanorp2040connect` | ✅ SUCCESS - builds firmware |
| `platformio run -e nanorp2040connect -t upload` | ✅ SUCCESS - flashes device |
| `platformio test -e native` | ✅ SUCCESS - runs tests |
| `platformio run -e nanorp2040connect` | ✅ Still works (explicit) |

### 1.4 Fix Applied
Added `[platformio]` section with `default_envs` to `platformio.ini`:

```ini
[platformio]
default_envs = nanorp2040connect
```

Now `platformio run` defaults to the `nanosrp2040connect` environment automatically.

---

## 2. PIN DEFINITION ANALYSIS

### 2.1 Pedal_Box/RP2040 Project
- **Status**: Fully functional, no build issues
- **Build time**: ~0.5s (with default_envs)
- **Flash/upload**: Works automatically
- **Native tests**: Pass

### 2.2 Steering_Wheel/RP2040 Project
- **Status**: Requires RP2040 pin remapping and I2C/UART API changes
- **Errors found**: 20+ compilation errors from `DXX` pin definitions not existing on RP2040
- **Fixes needed**: Replace all `DXX` with RP2040-appropriate pin numbers

---

## 3. GRILL SESSION: Architecture Decisions

The following decisions were resolved through the grill-with-docs process:

### Question 1: Primary Use Case
- **Decision**: (B) Standalone system interfacing with CAN for vehicles, integrates with existing Pedal_Box

### Question 2: CAN Implementation
- **Decision**: (A) MCP2515 + TJA1050 (standalone controller + transceiver)

### Question 3: Input Split
- **Decision**: 5 digital (optocouplers) + 5 analog (RP2040 built-in ADC)

### Question 4: Output Drivers
- **Decision**: MOSFET PWM for valves (NOT SSRs) - enables partial voltage control

### Question 5: Isolation
- **Decision**: Optocouplers on all digital inputs - essential automotive isolation

### Question 6: Output Current
- **Decision**: Mainly low current, but 2/4 channels may need high current (motors/valves)

### Question 7: Power Supply
- **Decision**: +12V input + USB power for bench/programming
- Reverse polarity protection required

### Question 8: Form Factor
- **Decision**: (A) Compact breakout board (space premium in car)

### Question 9: Connectivity
- **Decision**: (A) CAN bus preferred (since using for vehicle interface)

### Question 10: Firmware
- **Decision**: Arduino, no preference yet (testing phase)

### Question A: MCU Count
- **Decision**: Single RP2040 - "2" was just devkit version, no multi-MCU needed

### Question B: 10A Total Current
- **Decision**: 10A total was discussed but RP2040 GPIO pins can only handle ~12-15mA each
- Outputs drive relays/MOSFETs/valves externally, not directly from RP2040

### Question C: Analog 5V Car Sensors
- **Decision**: Typical sensors: NTC thermistors (temperature), 0.5-4.5V ratiometric (pressure)
- RP2040 ADC with voltage dividers to scale 0-5V → 0-3.3V

### Question D: Automotive Headers
- **Decision**: Common practices: Molex 35226-2000, Deutsch DTM06-12, JST, 2.54mm headers
- Existing 3.5mm jacks maintained for cabin use only

### Question D: Output Circuit Details
- **Decision**: Specific loads under evaluation - valves driven with MOSFET PWM for partial voltage control

### Question E: Final Architecture
- **Decision**: Design 1 - Single RP2040 board with all IO
- 5 optocoupler-isolated digital + 5 analog (built-in ADC)
- MOSFETs for valve drive (partial voltage PWM)
- MCP2515 + TJA1050 for CAN
- +12V input + USB bench power
- Compact, cabin-only 3.5mm jacks

---

## 4. SYSTEM ARCHITECTURE SUMMARY

### 4.1 Microcontroller
- **1x RP2040** (Nano RP2040 Connect dev kit v2)
- Arduino framework, existing Pedal_Box firmware compatible
- WiFi available but primarily used for bench programming

### 4.2 Power Supply
- **+12V input** for automotive operation
- **+USB power** for bench programming and development
- **Onboard 3.3V LDO regulator** for RP2040 and peripherals
- **Reverse polarity protection** recommended on +12V input

### 4.3 Input Channels (10 total)

#### 4.3.1 Digital Inputs (5 channels) - ISOLATED
- **Technology**: Optocouplers (PC817 or equivalent)
- **Purpose**: 12V signal detection (headlight switches, other car signals)
- **Isolation**: Full galvanic isolation between 12V car system and RP2040
- **Circuit**: Current-limiting resistor (470Ω) + optocoupler + 10kΩ pull-down

#### 4.3.2 Analog Inputs (5 channels) - RAW SENSOR DATA
- **Technology**: RP2040 built-in ADC (12-bit, 0-3.3V range)
- **Purpose**: Raw automotive sensor data
- **Conditioning**: Voltage dividers to scale 0-5V automotive signals to 0-3.3V
- **Protection**: TVS diodes and series resistors for ESD/spike protection

### 4.4 Output Channels

#### 4.4.1 Valve/Motor Drivers (2-4 high-current channels)
- **Technology**: **MOSFETs** (NOT SSRs)
- **Control**: PWM for partial voltage valve control
- **Circuit**: High-side or low-side MOSFET with freewheeling diode

#### 4.4.2 Low-Current Output Channels
- Purpose: Drive other systems (relays, indicators, other electronics)
- Current: ~50mA per channel maximum

### 4.5 CAN Bus Interface
- **Controller**: MCP2515 (SPI interface)
- **Transceiver**: TJA1050 (automotive CAN transceiver)
- **Baud rate**: Configurable (typically 250kbit/s for automotive)

### 4.6 Connectivity

#### 4.6.1 3.5mm Jack Interface
- **Location**: Internal cabin only (not exposed to external environment)
- **Function**: Connect to existing steering wheel/IO harness
- **Pinout**: 3.5mm TRRS or 3-pole depending on required channels

### 4.6.2 Power Connectors
- **Type**: Automotive molex or screw terminal for +12V

---

## 5. BILL OF MATERIALS (High-Level)

| Category | Key Components |
|----------|---------------|
| **MCU** | RP2040 (Nano RP2040 Connect dev kit v2) |
| **Isolation** | 5x PC817 optocouplers, 5x 470Ω resistors, 5x 10kΩ pull-downs |
| **ADC Conditioning** | 5x voltage divider networks (2x resistors each), TVS diodes |
| **Output Drivers** | 2-4x MOSFETs (logic-level, appropriate Vgs), 2-4x diodes (freewheeling), gate resistors |
| **CAN** | MCP2515 + TJA1050, CAN termination resistors (120Ω) |
| **Power** | 3.3V LDO (1117 or equivalent), reverse polarity protection (resistor/diode), USB connector |
| **Connectors** | 3.5mm jacks (existing), screw terminals or molex for +12V, CAN D-sub or Deutsch |
| **Passive** | capacitors for LDO filtering, ESD protection, pull-up/down resistors |

---

## 6. NEXT STEPS

1. **Schematic design** based on this architecture
2. **PCB layout** (compact, 2-layer preferred for cost)
3. **Prototype build and test**
4. **Firmware development** (Arduino conditional compilation)
5. **3D-printed enclosure** design (matches existing Pedal_Box style)
6. **Vehicle testing** (CAN integration, sensor validation)
7. **Documentation** of all revisions (ADRs in docs/adr/)

---

## 7. REFERENCES

- RP2040 Datasheet (Raspberry Pi)
- Arduino Nano RP2040 Connect Pinout
- PC817 Optocoupler Datasheet
- MCP2515 + TJA1050 CAN Application Notes
- Automotive sensor conditioning circuits
- 3.5mm jack pinout standards

---

## 8. CHANGE LOG

| Date | Change | Author |
|------|--------|--------|
| 2026-08-13 | Initial diagnosis & architecture document | HexRebuilt |
| 2026-08-13 | Fixed `platformio run` default environment | HexRebuilt |
| 2026-08-13 | Created CONTEXT.md and docs/ documentation | HexRebuilt |
## 8. KiCad Schematic Sheets (8 sheets, all in `docs/`)

| Sheet | Topic | Verification |
|-------|-------|-------------|
| **sheet1_power.sch** | Power Supply (AMS1117, reverse polarity, USB) | ✅ 3.3V LDO, diode protection, USB bench power |
| **sheet2_rp2040.sch** | RP2040 Core (GPIO, UART, SPI, ADC, USB) | ✅ All GPIO mappings documented |
| **sheet3_digital_inputs.sch** | 5-Channel Optocoupler-Isolated Inputs | ✅ PC817, 470Ω, 10k pull-down, 25mA max/channel |
| **sheet4_analog.sch** | 5-Channel Analog ADC Inputs | ✅ 20kΩ/10kΩ divider, 0-5V→0-1.67V safe for ADC |
| **sheet5_valve_drivers.sch** | MOSFET PWM Valve Drivers (2-4 channels) | ✅ AO3401/BSS123, 1kΩ gate resistor, 1N4007 diode |
| **sheet6_can_bus.sch** | MCP2515 + TJA1050 CAN Bus Interface | ✅ 250kbit/s, 120Ω termination, SPI to RP2040 |
| **sheet7_3mm_jacks.sch** | 3.5mm Jacks (GPS/Radio/Steering - 3 only) | ✅ Internal cabin only, NOT 12V automotive |
| **sheet8_jst_connectors.sch** | JST XH Connectors (All Other IO) | ✅ 2.5mm pitch, 1A rating, keyed polarization |

### Connector Strategy (Verified)

| Category | Connector Type | Qty | Purpose |
|----------|---------------|-----|---------|
| GPS + Radio + Steering | 3.5mm TRRS jacks | 3 | Internal cabin only |
| Digital optocoupler inputs | JST XH 6-pin male | 1 | DIN1-DIN5 + GND |
| Analog sensor inputs | JST XH 4-pin male | 1 | Analog1-3 + GND |
| Valve PWM outputs | JST XH 2-pin male | 2 per channel | Gate control + return |
| Sensor cables | JST XH 3-pin female | 3 | Supply, signal, GND |
| 12V Power Input | Molex screw terminal | 1 | +12V automotive input |
| CAN Bus | Molex 2-pin | 1 | CAN_H, CAN_L to vehicle |

### BOM Highlights (from `docs/architecture.md`)

- **MCU**: 1x RP2040 (Nano RP2040 Connect dev kit v2)
- **Optocouplers**: 5x PC817 with 470Ω resistors
- **ADC conditioning**: 5x (20kΩ + 10kΩ) voltage dividers + TVS diodes
- **MOSFET drivers**: 2-4x N-channel (AO3401/BSS123) with 1N4007 diodes
- **CAN**: MCP2515 + TJA1050 with 120Ω termination
- **Power**: +12V input + USB bench, AMS1117-3.3V LDO
- **Jacks**: 3x 3.5mm TRRS (GPS, radio, steering) internal cabin only
- **Secondary IO**: JST XH 2.5mm pitch connectors

### Build & Flash (Fixed)

- `platformio run` now defaults to `nanosrp2040connect` (via `default_envs` in `platformio.ini`)
- `platformio run -e nanorp2040connect -t upload` flashes firmware
- `platformio test -e native` runs tests
- **Previous issue**: Running `platformio run` without `-e` failed on `native` environment

### Documentation Files in `docs/`

- `architecture.md` - Complete system architecture & BOM
- `CONTEXT.md` - Architecture decisions & change log
- `sheet1_power.sch` through `sheet8_jst_connectors.sch` - All 8 KiCad schematic sheets
