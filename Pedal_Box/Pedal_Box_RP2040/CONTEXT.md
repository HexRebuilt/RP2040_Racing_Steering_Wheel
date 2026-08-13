# RP2040 Automotive IO Board - Architecture Decision Document

## Version: 1.0
## Date: 2026-08-13
## Author: HexRebuilt

## 1. SYSTEM OVERVIEW

A compact RP2040-based IO board for automotive cabin use, designed to interface with:
- 5 optocoupler-isolated digital inputs (12V signals)
- 5 analog inputs (raw automotive sensor data)
- Valve/motor outputs with PWM partial voltage control
- CAN bus for vehicle communication
- 3.5mm jack connectivity (internal cabin only)

**Primary use**: Standalone IO expansion for RP2040 Racing Steering Wheel Pedal_Box, with vehicle CAN integration.

## 2. HARDWARE ARCHITECTURE

### 2.1 Microcontroller
- **1x RP2040** (Nano RP2040 Connect dev kit v2)
- "The '2' refers to devkit version, not two MCUs"
- Arduino framework, existing Pedal_Box firmware compatible
- WiFi available but primarily used for bench programming

### 2.2 Power Supply
- **+12V input** for automotive operation
- **+USB power** for bench programming and development
- **Onboard 3.3V LDO regulator** for RP2040 and peripherals
- **Reverse polarity protection** recommended on +12V input
- **Undervoltage/overvoltage protection** for 10.5V-16V range

### 2.3 Input Channels (10 total)

#### 2.3.1 Digital Inputs (5 channels) - ISOLATED
- **Technology**: Optocouplers (PC817 or equivalent)
- **Purpose**: 12V signal detection (headlight switches, other car signals)
- **Isolation**: Full galvanic isolation between 12V car system and RP2040
- **Input circuit**: 
  - Current-limiting resistor (470Ω) from 12V to optocoupler anode
  - Optocoupler cathode to RP2040 GPIO pin
  - 10kΩ pull-down resistor to ground
- **Logic**: Optocoupler ON = 12V present → GPIO reads LOW (with INPUT_PULLUP) or HIGH (configured)

#### 2.3.2 Analog Inputs (5 channels) - RAW SENSOR DATA
- **Technology**: RP2040 built-in ADC (12-bit, 0-3.3V range)
- **Purpose**: Raw automotive sensor data (temperature, pressure, voltage)
- **Input conditioning**: Voltage dividers to scale 0-5V automotive signals to 0-3.3V
- **Protection**: TVS diodes and series resistors for ESD/spike protection
- **Sensors expected**:
  - Temperature: NTC thermistors (common automotive)
  - Pressure: 0.5-4.5V ratiometric (MAP, fuel pressure sensors)
  - Direct voltage sensing (with attenuation)

### 2.4 Output Channels

#### 2.4.1 Valve/Motor Drivers (2-4 high-current channels)
- **Technology**: **MOSFETs** (NOT SSRs - MOSFETs selected)
- **Control**: PWM for partial voltage valve control
- **Purpose**: Drive valve coils with variable partial voltage to open/close
- **Circuit**: High-side or low-side MOSFET with freewheeling diode
- **Current**: Per-channel rating must accommodate valve coil current (typically 200-500mA per valve)
- **Total system current**: Well under 10A (10A was a conservative worst-case estimate)

#### 2.4.2 Low-Current Output Channels
- Purpose: Drive other systems (relays, indicators, other electronics)
- Current: ~50mA per channel maximum
- Technology: Direct GPIO drive or MOSFET for isolated channels

### 2.5 CAN Bus Interface
- **Controller**: MCP2515 (SPI interface)
- **Transceiver**: TJA1050 (automotive CAN transceiver)
- **Purpose**: Vehicle bus communication (engine data, sensor values, commands)
- **Physical layer**: CAN_H and CAN_L lines to vehicle diagnostic connector
- **Baud rate**: Configurable (typically 250kbit/s for automotive)

### 2.5 Connectivity

#### 2.5.1 3.5mm Jack Interface
- **Quantity**: Existing jacks (3-pole or TRRS)
- **Location**: Internal cabin only (not exposed to external environment)
- **Function**: Connect to existing steering wheel/IO harness
- **Pinout**: 3.5mm TRRS or 3-pole depending on required channels
- **Signal types**: Analog audio/data, digital switch signals

#### 2.5.2 CAN Connectors
- **Type**: Automotive Deutsch/DTM or equivalent
- **Purpose**: Vehicle bus connection

#### 2.5.3 Power Connectors
- **Type**: Automotive molex or screw terminal for +12V
- **Purpose**: Power input (+12V + ground)

## 3. SOFTWARE ARCHITECTURE

### 3.1 Firmware Framework
- **Arduino core** for RP2040
- Existing Pedal_Box firmware as base
- Conditional compilation for new features

### 3.2 Main Functions
- **CAN bus processing**: Receive/send vehicle data
- **Input scanning**: 5 isolated digital + 5 analog channels
- **Output control**: PWM valve driving, low-current outputs
- **3.5mm jack handling**: Existing Pedal_Box communication
- **USB interface**: Programming and bench operations

### 3.3 Communication Flow
1. **Vehicle → Board**: CAN messages (sensor data, commands)
2. **Board → Cabin**: 3.5mm jack signals (existing Pedal_Box protocol)
3. **Board ↔ Bench**: USB serial for programming/monitoring
4. **Board → Outputs**: MOSFET PWM for valve control

## 4. DESIGN CONSTRAINTS & RATIONALE

### 4.1 Why Single RP2040 (Not 2 or 3)?
- "The '2' is the devkit version" - One MCU is sufficient
- All functions fit within RP2040 capabilities (264KB RAM, 133MHz)
- CAN, ADC, optocoupler reading, PWM output all manageable
- Reduces complexity, cost, and power consumption
- Easier firmware maintenance single codebase

### 4.2 Why MOSFETs Over SSRs for Valves?
- **MOSFETs**: Allow PWM partial voltage control (vary open amount)
- **SSRs**: Binary on/off, not suitable for partial valve control
- MOSFETs: More efficient, faster switching, lower cost
- Application: Valve coils energized with PWM to control flow rate

### 4.3 Why Optocoupler Isolation on Digital Inputs?
- **Car electrical system is "nasty"**: 12V-14.5V running, load dump spikes, ESD
- **Direct exposure destroys RP2040 GPIO**: 5V tolerant but not 12V proof
- **Optocouplers provide**: Galvanic isolation, spike absorption, safety margin
- **Cost/space**: Minimal (1 chip + 3 resistors per channel)

### 4.4 Why Voltage Dividers for Analog Inputs?
- **Raw 5V automotive sensors**: Temperature, pressure sensors output 0-5V typical
- **RP2040 ADC max**: 3.3V - need scaling
- **Voltage divider**: 2 resistors per channel (simple, cheap)
- **Protection**: Required to prevent ADC damage from transient spikes

### 4.5 Form Factor Rationale
- **Compact**: For car installation where space is premium
- **3.5mm jacks**: Leverage existing harness, no new connector tooling
- **Mounting holes**: For 3D-printed enclosure or panel mount
- **Cabin-only**: Connectors stay inside, not exposed to engine bay conditions

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

## 6. NEXT STEPS

1. **Schematic design** based on this architecture
2. **PCB layout** (compact, 2-layer preferred for cost)
3. **Prototype build and test**
4. **Firmware development** (Arduino conditional compilation)
5. **3D-printed enclosure** design (matches existing Pedal_Box style)
6. **Vehicle testing** (CAN integration, sensor validation)
7. **Documentation** of all revisions (ADRs in docs/adr/)

## 7. REFERENCES

- RP2040 Datasheet (Raspberry Pi)
- Arduino Nano RP2040 Connect Pinout
- PC817 Optocoupler Datasheet
- MCP2515 + TJA1050 CAN Application Notes
- Automotive sensor conditioning circuits
- 3.5mm jack pinout standards
## 8. Schematics & PCB Design

### KiCad Schematic Sheets (8 sheets, all in `docs/`)

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