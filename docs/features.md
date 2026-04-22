# Features and Planned Improvements

This document outlines the currently implemented features and identifies areas for future enhancement.

## Implemented Features

### Steering Wheel (Raspberry Pi Pico)

#### Input Features
- **Rotary Encoder Support**
  - 2x KY-040 encoders with full quadrature decoding
  - Volume control encoder (Encoder 1)
  - Menu navigation encoder (Encoder 2)
  - Push-button functionality on each encoder

- **Button Input**
  - 9 push buttons configured with internal pull-ups
  - 6 radio control buttons (Red, Blue, Green, Black, Next, Back)
  - 3 ECU buttons (Red, Yellow, White)
  - 2 rocker switches for menu up/down navigation
  - Button press/release detection with debouncing

#### Output Features
- **LCD Display**
  - 8-digit 7-segment display via MAX7219
  - LedControl library integration
  - Displays: speed, time, satellite count, heading

- **LED Shift Light**
  - NeoPixel bar with 8 LEDs
  - Partially configured in code
  - Ready for RPM-based activation

- **Ambient Light Sensing**
  - BH1750 I2C light sensor
  - Automatic display brightness adjustment

#### Communication
- UART1 interface to Pedal Box (9600 baud)
- Protocol: ASCII message format with newline termination

### Pedal Box (Arduino Nano RP2040 Connect)

#### Input Features
- **GPS Module**
  - TinyGPS++ library integration
  - Parses NMEA sentences
  - Extracts: time, speed, position, heading, satellite count

#### Output Features
- **Digital Potentiometer**
  - MCP4131 SPI-controlled volume
  - 0-100% volume range
  - Radio volume adjustment

#### Communication
- UART0 to Steering Wheel (9600 baud)
- UART1 to GPS module
- GPS data forwarding to steering wheel

## Known Bugs

### 1. GPS Data Not Parsed on Steering Wheel

**Bug ID**: BUG-001
**Severity**: High
**Affected File**: Steering_Wheel/Stering_Wheel_RP2040/src/main.cpp
**Description**: The Steering Wheel receives GPS data from the Pedal Box via UART1 but does not parse or display it. The message is read (lines 216-229) but discarded without extracting speed, satellite count, or time values.

**Current Behavior**:
- Pedal Box correctly sends: `g.sp.45 g.st.8 g.h.14 g.m.30 g.s.25\n`
- Steering Wheel reads messageIn but never extracts values
- LCD continues showing default/tachometer mode only

**Expected Behavior**:
- Extract speed from `g.sp.X`
- Extract satellite count from `g.st.X`  
- Extract time from `g.h.X g.m.X g.s.X`
- Display on LCD in appropriate modes

**Root Cause**: Code comment indicates "use sscanf to detect the parts" but implementation is missing.

**Fix Required**:
1. Add GPS data parsing using sscanf or string parsing
2. Store extracted values in Lcd8Digit class
3. Update display modes to show GPS data
4. Test integration with Pedal Box GPS output

### 2. Persistent Storage Not Implemented

**Bug ID**: BUG-002
**Severity**: Medium
**Affected File**: Steering_Wheel/Stering_Wheel_RP2040/src/apps/HumanInterface/Lcd8Digit.h
**Description**: StorageManager class is referenced but commented out. User settings (volume, timezone, brightness) are lost on power cycle.

**Current Behavior**: StorageManager code commented out (lines 2, 8, 200, 215-218, 307-308)
**Expected Behavior**: Settings persist across power cycles

## Missing / Incomplete Features

### 1. Real-Time Clock (RTC)

**Current State**: No RTC chip installed
**Impact**: GPS-derived time is not persisted; time lost on power cycle

**Planned Solution**:
- Add DS3231 RTC module via I2C
- Sync time from GPS on initial fix
- Maintain time during GPS outages

**Implementation Notes**:
- I2C pins available: SDA=GP2, SCL=GP3 (Pico)
- DS3231 library available from Arduino library manager
- Battery backup for time persistence

### 2. Speed-Based Volume Control

**Current State**: Volume controlled manually via encoder
**Impact**: No automatic volume adjustment based on vehicle speed

**Planned Solution**:
- Monitor GPS speed from Pedal Box
- Adjust volume inversely to speed (louder at higher speeds)
- User-configurable sensitivity levels

**Implementation Notes**:
- Add speed threshold configuration
- Smooth volume transitions to avoid jarring changes
- Store last volume setting in persistent storage

### 3. Persistent Storage

**Current State**: StorageManager referenced in code but commented out
**Impact**: Settings lost on power cycle; no calibration data persistence

**Planned Solution**:
- Implement EEPROM/Flash storage
- Store: volume level, display mode, calibration data
- Use RP2040 flash memory or external I2C EEPROM

**Implementation Notes**:
- Check for StorageManager class in codebase
- Implement save/load functions
- Add default value handling

### 4. RPM Input

**Current State**: Pin defined but not connected to any sensor
**Impact**: No engine RPM display; shift light not functional

**Planned Solution**:
- Connect RPM signal source (simulator or ECU)
- Implement frequency counting for RPM calculation
- Connect to shift light for visual feedback

**Implementation Notes**:
- Verify RPM pin in pinout definition
- Check for interrupt-based frequency measurement
- Map RPM to LED bar for shift light

### 5. LED Bar Shift Light

**Current State**: Configured but disabled in code
**Impact**: No visual RPM indicator

**Planned Solution**:
- Enable NeoPixel control code
- Connect RPM input (see above)
- Define shift points (e.g., 4000, 5000, 6000, 7000 RPM)
- Progressive LED activation

**Implementation Notes**:
- Verify NeoPixel data pin connection
- Define color scheme (green -> yellow -> red)
- Add user-configurable shift points

## Feature Priority Matrix

| Priority | Feature | Complexity | Impact |
|----------|---------|------------|--------|
| High | GPS Data Parsing (BUG-001) | Medium | Core functionality |
| High | RPM Input + Shift Light | Medium | Core functionality |
| High | Persistent Storage (BUG-002) | Medium | User experience |
| Medium | Speed-based Volume | Low | User experience |
| Medium | RTC | Medium | Time keeping |
| Low | Protocol Enhancement | Low | Reliability |

## Development Roadmap

### Phase 1: Core Functionality (In Progress)
- [x] Button input processing
- [x] Encoder reading
- [x] LCD display output
- [x] UART communication
- [x] GPS data display

### Phase 2: User Experience (Next)
- [ ] Persistent storage for settings
- [ ] Speed-based volume control
- [ ] Automatic brightness adjustment

### Phase 3: Advanced Features
- [ ] RPM input implementation
- [ ] LED shift light activation
- [ ] RTC integration
- [ ] Protocol improvements

## Testing Checklist

- All buttons register correctly
- Encoders increment/decrement properly
- LCD displays all data fields
- GPS data parses correctly
- UART communication stable
- Volume control via MCP4131 works
- Light sensor reading functional

## Known Issues

1. LED bar not activated - RPM input missing
2. Time not persisted - no RTC
3. Settings reset on power cycle - no persistent storage
4. Volume only manual - no speed-based auto-adjust

## External Dependencies

- [LedControl](https://github.com/wayoda/LedControl) - MAX7219 display driver
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - LED bar control
- [TinyGPS++](http://arduiniana.org/libraries/tinygpsplus/) - GPS parsing
- [BH1750](https://github.com/claws/BH1750) - Light sensor library

## Speed-Based Automatic Volume Control - Implementation Specification

### Feature Overview
Automatically adjust radio volume based on vehicle speed. When speed exceeds a configurable threshold, volume increases. When speed drops below another threshold, volume decreases.

### User Requirements (from user input)
- Configurable speed threshold (default: 70 km/h)
- Configurable volume increment/decrement amount (default: 1 toggle per event)
- Volume decreases when speed drops below threshold by configurable interval (default: 5 km/h)
- Remember last speed that triggered volume change to avoid GPS jitter issues

### Data Flow
1. Pedal Box reads GPS speed continuously
2. Pedal Box sends speed data to Steering Wheel via UART (already implemented as "g.sp.X")
3. Steering Wheel receives and stores speed
4. Steering Wheel runs speed-to-volume logic
5. Steering Wheel sends volume change commands back to Pedal Box
6. Pedal Box applies change to MCP4131

### Implementation Details

#### Configuration Parameters (stored in EEPROM/Flash)
```cpp
// Default values - user configurable
#define DEFAULT_SPEED_THRESHOLD      70    // km/h
#define DEFAULT_SPEED_HYSTERESIS      5     // km/h interval for volume decrease
#define DEFAULT_VOLUME_INCREMENT     1     // MCP4131 steps per event
#define MIN_SPEED_THRESHOLD           20    // km/h
#define MAX_SPEED_THRESHOLD           200   // km/h
```

#### State Machine (Steering Wheel)
```
States:
- IDLE: Normal manual volume control
- MONITORING: Speed exceeds threshold, auto-volume active
- HYSTERESIS: Speed dropped below (threshold - hysteresis), waiting

Events:
- Speed > threshold -> Enter MONITORING, record triggerSpeed
- Speed drops below (triggerSpeed - hysteresis) -> Decrease volume, update triggerSpeed
- User manual volume adjustment -> Exit auto mode, enter IDLE
```

#### GPS Jitter Handling
- Use rolling average of last 3-5 speed readings
- Minimum time between volume changes: 2 seconds (debounce)
- Store lastTriggerSpeed in persistent storage
- Only trigger volume change when: (currentSpeed - lastTriggerSpeed) >= hysteresis AND timeSinceLastChange > 2s

#### Inter-Board Protocol Extension
Add new message types:
```
V.XX    - Set volume directly (XX = 0-127)
V+      - Volume increment
V-      - Volume decrement  
A.ON    - Enable auto-volume
A.OFF   - Disable auto-volume
S.THR   - Set speed threshold (S.THR.70)
S.HYS   - Set hysteresis (S.HYS.5)
```

### Affected Files
1. Steering_Wheel/Stering_Wheel_RP2040/src/main.cpp - Add speed processing
2. Steering_Wheel/Stering_Wheel_RP2040/src/apps/HumanInterface/Lcd8Digit.h - Add auto-volume display mode
3. Pedal_Box/Pedal_Box_RP2040/src/main.cpp - Add new protocol handlers
4. docs/features.md - Add this specification

## RTC and GPS Time Synchronization - Implementation Specification

### Feature Overview
Use GPS time to synchronize and maintain accurate clock, with optional RTC module for backup when GPS signal is unavailable.

### Current State
- GPS time is received and displayed on LCD
- Time is NOT persisted - lost on power cycle
- No RTC chip installed

### Implementation Options

#### Option A: GPS-Only (No Hardware Change)
- Use GPS time as primary time source
- Store last valid time in flash memory
- On startup, if GPS has fix, sync immediately
- If no GPS, use last stored time (with "stale" indicator)

#### Option B: Hardware RTC (Recommended)
Add DS3231 RTC module:
- I2C interface (SDA=GP2, SCL=GP3 on Pico - same as BH1750)
- Maintains time during power loss (coin cell battery)
- Syncs from GPS on startup and periodically

### Implementation Details

#### Hardware (Option B)
- DS3231 RTC module
- Connect to Pico I2C pins (shared with BH1750 - OK since I2C addresses differ)
- Add coin cell battery for backup

#### Software Architecture
```cpp
class TimeManager {
private:
    bool gpsTimeValid;
    bool rtcPresent;
    int timezoneOffset;  // User-configurable
    unsigned long lastGpsSync;
    
public:
    void Initialize();
    bool SyncFromGPS(int year, int month, int day, int hour, int minute, int second);
    void SyncToRTC();
    void GetTime(int& hour, int& minute, int& second);
    bool IsGPSValid();
};
```

#### Sync Logic
1. On startup: Check RTC, check GPS
2. If GPS has fix (satellites > 0) AND time valid:
   - If RTC not present: Use GPS time, store in flash
   - If RTC present: Sync RTC from GPS, use RTC as reference
3. Every 10 minutes: Re-sync RTC from GPS if fix available
4. Display shows "GPS" indicator when time from GPS, "RTC" when from RTC

#### Timezone Handling
- Store timezone offset in persistent storage
- Adjust display time: displayTime = utcTime + timezoneOffset
- User can adjust via menu encoder (see Lcd8Digit.h ModifyValues)

#### Time Display Format
- 24-hour format: HH-MM
- Seconds hidden by default, shown in clock mode
- Date not currently displayed (consider adding)

### Affected Files
1. Steering_Wheel/Stering_Wheel_RP2040/src/apps/HumanInterface/TimeManager.h (new)
2. Steering_Wheel/Stering_Wheel_RP2040/src/main.cpp - Initialize TimeManager
3. docs/features.md - Add this specification