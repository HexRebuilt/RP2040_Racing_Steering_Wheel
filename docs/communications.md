# Inter-Board Communication Protocol

This document describes the communication protocol between the Steering Wheel and Pedal Box units.

## Overview

The two boards communicate via UART using a simple ASCII-based message protocol. The Steering Wheel acts as the primary controller, sending button events to the Pedal Box, which in turn relays GPS data back to the Steering Wheel for display.

## Physical Connection

| Parameter | Value |
|-----------|-------|
| Interface | UART |
| Baud Rate | 9600 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |

### Pin Mapping

- **Steering Wheel (Pico)**: UART1
  - TX: GP0 (Pin 1)
  - RX: GP1 (Pin 2)

- **Pedal Box (Nano RP2040)**: UART0
  - TX: p0 (Pin 1)
  - RX: p1 (Pin 2)

## Message Format

All messages are ASCII text strings terminated with a newline character (`\n`). The protocol uses a simple request-response pattern with periodic broadcast messages.

### Direction: Steering Wheel to Pedal Box

#### Button Event Messages

Format: `BUTTON:<button_id>\n`

The Steering Wheel sends button press and release events to the Pedal Box.

**Button ID Mapping:**

| ID | Button Name | Location |
|----|-------------|----------|
| 1 | Radio Red | Radio cluster |
| 2 | Radio Blue | Radio cluster |
| 3 | Radio Green | Radio cluster |
| 4 | Radio Black | Radio cluster |
| 5 | Radio Next | Radio cluster |
| 6 | Radio Back | Radio cluster |
| 7 | ECU Red | ECU panel |
| 8 | ECU Yellow | ECU panel |
| 9 | ECU White | ECU panel |
| 10 | Menu Up | Rocker switch |
| 11 | Menu Down | Rocker switch |
| 12 | Encoder 1 Push | Volume encoder |
| 13 | Encoder 2 Push | Menu encoder |

**Message Examples:**

```
BUTTON:1\n      // Radio Red pressed
BUTTON:0:1\n    // Radio Red released
BUTTON:7\n      // ECU Red pressed
BUTTON:0:7\n    // ECU Red released
```

#### Volume Control Messages

Format: `VOLUME:<value>\n`

Sent when the volume encoder is rotated. Value range: 0-100.

**Message Example:**

```
VOLUME:75\n
```

### Direction: Pedal Box to Steering Wheel

#### GPS Data Messages

Format: `g.sp.<speed> g.st.<satellites> g.h.<heading> g.m.<lat> g.s.<lon>\n`

The Pedal Box broadcasts GPS data at regular intervals (typically 1 second).

**Field Definitions:**

| Field | Meaning | Example |
|-------|---------|---------|
| g.sp | Speed (km/h) | g.sp.45 |
| g.st | Satellite count | g.st.8 |
| g.h | Heading (degrees) | g.h.180 |
| g.m | Latitude | g.m.51.5074 |
| g.s | Longitude | g.s.-0.1278 |

**Message Example:**

```
g.sp.45 g.st.8 g.h.180 g.m.51.5074 g.s.-0.1278\n
```

#### Time Messages

Format: `g.h.<hour> g.m.<minute> g.s.<second>\n`

GPS-derived time is sent separately from position data.

**Message Example:**

```
g.h.14 g.m.30 g.s.25\n
```

## Communication Flow

### Initialization Sequence

1. Pedal Box starts and initializes GPS module
2. Pedal Box begins broadcasting GPS data every 1 second
3. Steering Wheel starts and connects to Pedal Box via UART
4. Steering Wheel displays "CONNECTED" or waits for first GPS data

### Normal Operation

```
┌─────────────────────┐         UART         ┌─────────────────────┐
│   Steering Wheel   │◄──────────────────────►│    Pedal Box       │
│                     │                       │                     │
│  Read buttons ─────►│── BUTTON:7 ─────────►│  Process button    │
│                     │                       │  Update volume     │
│                     │◄── g.sp.45 g.st.8 ────│  Parse GPS data    │
│  Update display ◄───│                       │  Broadcast GPS     │
│                     │                       │                     │
└─────────────────────┘                       └─────────────────────┘
```

### Button Handling

1. User presses button on Steering Wheel
2. Steering Wheel detects press, sends `BUTTON:<id>` to Pedal Box
3. Pedal Box receives and processes button (e.g., adjusts volume)
4. When button released, Steering Wheel sends `BUTTON:0:<id>`
5. Pedal Box receives release event

## Error Handling

### GPS Data Timeout

If no valid GPS data received for >5 seconds:
- Display shows "NO GPS" or last known values flash
- Time display shows "--:--:--" or last known time

### UART Disconnection

If no communication received for >2 seconds:
- Steering Wheel may show connection status
- Pedal Box continues GPS polling independently

### Data Validation

- Speed values clamped to 0-300 km/h range
- Latitude clamped to -90 to 90 degrees
- Longitude clamped to -180 to 180 degrees
- Satellite count: 0-20 (displays as "NO FIX" if 0)

## Timing Considerations

| Parameter | Value |
|-----------|-------|
| GPS broadcast interval | 1000 ms |
| Button event debounce | 50 ms |
| UART receive buffer | 128 bytes |
| Message timeout | 2000 ms |

## Future Protocol Enhancements

Potential improvements for future versions:

1. **ACK/NACK Protocol**: Add acknowledgment for critical messages
2. **Configuration Messages**: Bidirectional setup and calibration
3. **RPM Data**: Add RPM field to status messages
4. **checksum**: Add simple checksum for data integrity

## Implementation Notes

- Use `Serial1` on the Pico for UART1
- Use `Serial1` on Nano RP2040 for GPS, `Serial` for wheel communication
- Buffer incoming data until newline received
- Parse strings using `String` class or manual parsing
- Consider using `strtok()` for GPS message parsing