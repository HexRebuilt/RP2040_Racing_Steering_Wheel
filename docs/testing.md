# Testing Plan

This document outlines the testing procedures and validation methods for the RP2040 Racing Steering Wheel project.

## Test Environment

### Hardware Requirements
- Raspberry Pi Pico (Steering Wheel board)
- Arduino Nano RP2040 Connect (Pedal Box board)
- USB cables for both boards
- GPS module (TinyGPS++ compatible)
- Computer with PlatformIO installed
- Serial terminal software (Arduino IDE Serial Monitor, PuTTY, etc.)

### Software Requirements
- PlatformIO core
- Arduino framework for RP2040
- Serial terminal application

## Unit Tests

### Steering Wheel Unit Tests

#### 1. Button Input Test

**Objective**: Verify all buttons register correctly

**Procedure**:
1. Upload firmware to Pico
2. Open Serial Monitor at 115200 baud
3. Press each button sequentially
4. Observe button ID in serial output
5. Verify release events are also captured

**Expected Results**:
| Button | Press Output | Release Output |
|--------|--------------|----------------|
| Radio Red | BUTTON:1 | BUTTON:0:1 |
| Radio Blue | BUTTON:2 | BUTTON:0:2 |
| Radio Green | BUTTON:3 | BUTTON:0:3 |
| Radio Black | BUTTON:4 | BUTTON:0:4 |
| Radio Next | BUTTON:5 | BUTTON:0:5 |
| Radio Back | BUTTON:6 | BUTTON:0:6 |
| ECU Red | BUTTON:7 | BUTTON:0:7 |
| ECU Yellow | BUTTON:8 | BUTTON:0:8 |
| ECU White | BUTTON:9 | BUTTON:0:9 |
| Menu Up | BUTTON:10 | BUTTON:0:10 |
| Menu Down | BUTTON:11 | BUTTON:0:11 |
| Enc1 Push | BUTTON:12 | BUTTON:0:12 |
| Enc2 Push | BUTTON:13 | BUTTON:0:13 |

#### 2. Rotary Encoder Test

**Objective**: Verify encoder rotation produces correct output

**Procedure**:
1. Connect encoder to designated pins
2. Rotate encoder clockwise
3. Observe increment values
4. Rotate encoder counter-clockwise
5. Observe decrement values

**Expected Results**:
- Clockwise: Values increment (e.g., 1, 2, 3...)
- Counter-clockwise: Values decrement (e.g., -1, -2, -3...)
- Push button registers like other buttons

#### 3. LCD Display Test

**Objective**: Verify all display segments function

**Procedure**:
1. Run display test routine (if available)
2. Cycle through all display modes
3. Verify each digit can display 0-9
4. Check decimal points activate correctly

**Expected Results**:
- All 8 digits illuminate
- No stuck segments
- Brightness responds to light sensor

#### 4. Light Sensor Test

**Objective**: Verify BH1750 light sensor reading

**Procedure**:
1. Open Serial Monitor
2. Observe lux readings in serial output
3. Cover sensor (decrease light)
4. Expose sensor to bright light (increase)
5. Verify values change appropriately

**Expected Results**:
- Range: ~0 lux (dark) to ~65535 lux (direct sunlight)
- Values update every measurement cycle

### Pedal Box Unit Tests

#### 1. GPS Module Test

**Objective**: Verify GPS data acquisition

**Procedure**:
1. Place GPS module with clear sky view
2. Wait for satellite fix (may take 30-60 seconds)
3. Observe NMEA data in Serial Monitor
4. Check for valid latitude/longitude

**Expected Results**:
- Satellite count > 0
- Valid coordinates (non-zero)
- Time updates every second

#### 2. UART Communication Test

**Objective**: Verify message transmission to Steering Wheel

**Procedure**:
1. Connect Pedal Box to Steering Wheel via UART
2. Monitor GPS data on Steering Wheel serial output
3. Verify message format: `g.sp.X g.st.X g.h.X g.m.X g.s.X`

**Expected Results**:
- Messages arrive every ~1 second
- Format matches specification
- No garbled characters

#### 3. Digital Potentiometer Test

**Objective**: Verify MCP4131 volume control

**Procedure**:
1. Connect digital potentiometer to audio output
2. Send volume commands via UART
3. Verify audio level changes

**Expected Results**:
- Volume range: 0 (mute) to 100 (full)
- Smooth volume transitions

## Integration Tests

### 1. Full Communication Test

**Objective**: Verify bidirectional data flow

**Procedure**:
1. Power on both boards
2. Wait for GPS fix on Pedal Box
3. Press buttons on Steering Wheel
4. Observe Pedal Box response
5. Monitor GPS data on Steering Wheel display

**Expected Results**:
- Button events trigger appropriate responses
- GPS data displays on steering wheel LCD
- No communication dropouts

### 2. Display Data Flow Test

**Objective**: Verify GPS data displays correctly

**Procedure**:
1. Establish GPS fix with >4 satellites
2. Move (simulate movement or walk outside)
3. Observe speed changes on LCD
4. Check time display updates
5. Verify satellite count shows current status

**Expected Results**:
- Speed: 0-300 km/h range
- Time: HH:MM:SS format
- Satellites: 0-20 range

### 3. Button Response Test

**Objective**: Verify button press triggers volume control

**Procedure**:
1. Connect audio system to potentiometer output
2. Adjust volume to known level
3. Press volume encoder
4. Rotate encoder to change volume

**Expected Results**:
- Encoder rotation changes volume smoothly
- Volume changes reflected in audio output

## Stress Tests

### 1. Continuous Operation Test

**Objective**: Verify system stability over extended period

**Procedure**:
1. Run system for 8+ hours
2. Monitor for freezes or crashes
3. Check memory usage (if available)
4. Verify communication remains stable

**Expected Results**:
- No crashes
- Memory stable
- Communication uninterrupted

### 2. Rapid Button Press Test

**Objective**: Verify debouncing handles rapid input

**Procedure**:
1. Rapidly press multiple buttons
2. Verify no missed or duplicate events
3. Check system response remains consistent

**Expected Results**:
- Each press registers exactly once
- No spurious events

### 3. GPS Signal Loss Test

**Objective**: Verify graceful handling of GPS dropout

**Procedure**:
1. Establish GPS fix
2. Disconnect or shield GPS antenna
3. Wait for signal loss
4. Observe display behavior

**Expected Results**:
- Display shows last known values or "NO GPS"
- No crash or freeze
- Recovers when signal restored

## Debugging Procedures

### Serial Debug Output

Enable debug output by checking source code for `#define DEBUG` or similar flags. Common debug information:

- Button press/release events
- Encoder position changes
- GPS parsing status
- UART message contents
- Error conditions

### LED Status Indicators

If LED indicators are configured:
- Green: Normal operation
- Yellow: GPS searching
- Red: Error condition

### Common Issues

| Issue | Possible Cause | Solution |
|-------|-----------------|----------|
| No serial output | Incorrect baud rate | Set to 115200 |
| GPS not fixing | Weak signal | Move to open area |
| Buttons not responding | Wiring issue | Check connections |
| LCD blank | Brightness too low | Check light sensor |
| UART no data | Crossed TX/RX | Verify wiring |

## Test Equipment Checklist

- [ ] 2x micro USB cables
- [ ] Serial terminal software
- [ ] GPS antenna/module
- [ ] Multimeter (for voltage checks)
- [ ] Jumper wires for testing
- [ ] 3.3V/5V power source (if needed)

## Validation Checklist

Before considering the system ready for deployment, verify:

- [ ] All buttons register correctly
- [ ] Both encoders function properly
- [ ] LCD displays all required data
- [ ] GPS acquires fix within 2 minutes
- [ ] UART communication stable
- [ ] Volume control operational
- [ ] System runs without crashes for 1+ hour

## Test Reporting

Document test results in the following format:

```
Test Name: [Test name]
Date: [YYYY-MM-DD]
Result: [PASS/FAIL]
Notes: [Any observations or issues]
```

Create a test log file to track ongoing validation.