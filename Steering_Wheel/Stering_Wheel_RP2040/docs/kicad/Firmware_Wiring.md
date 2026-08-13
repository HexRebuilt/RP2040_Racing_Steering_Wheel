# RP2040 Racing Steering Wheel Firmware & Wiring Guide

## Board Layout & Pinout

### PCB Features (from docs/kicad/):
- **8 Button Holes + Pads** (B1-B8): Positions defined for future soldering
- **Reset Button Position** (RESET): Near RP2040 EN pin
- **Boot Button Position** (BOOT): Near GPIO22 for mass storage mode
- **LCD Area** (larger rectangle): 8-digit 7-segment SPI display (populate later)
- **LED Area** (smaller rectangle): WS2812B 8-LED strip (populate later)
- **Type-C USB**: Power + mass storage OTA
- **3.5mm TRRS Jack**: UART to pedal box
- **BH1750 I2C**: D2 (SDA), D3 (SCL)
- **2 KY-040 Encoders**: Menu and Volume

### Wiring Diagram: Adding Buttons Later

```
+----------------------+
|      RP2040          |
|                      |
|  +-----------+       |
|  | B1_B2_B3_B4|       |
|  | [ ] [ ] [ ] [ ]|  Buttons splice to pads |
|  +-----------+       |
|                      |
|  +-----------+       |
|  | B5_B6_B7_B8|       |
|  | [ ] [ ] [ ] [ ]|  Buttons splice to pads |
|  +-----------+       |
|                      |
|  +-----------+       |
|  |  Reset    |       |
|  |  [ ]        |  To EN pin (GPIO)  |
|  +-----------+       |
|                      |
|  +-----------+       |
|  |  Boot     |       |
|  |  [ ]        |  To GPIO22 (BootSEL) |
|  +-----------+       |
+----------+------------+
           |
           +--> Type-C USB (power + OTA mass storage)
           |
           +--> 3.5mm TRRS Jack (UART to pedal box)
           |
           +--> BH1750 I2C (D2, D3)
           |
           +--> WS2812B LED (D6 - DATA)
           |
           +--> 2 Encoders (A, B, Switch each)
           |
           +--> GND (common ground)

========================================

## Firmware: Arduino Sketch with Pin Definitions

Upload via:
1. **Mass Storage (BOOTSEL)**: Hold boot button, plug USB, drag .uf2
2. **Standard USB**: Press Reset, then Boot, then click Download
3. **UART OTA**: Via 3.5mm TRRS jack (advanced)

========================================
/*
 * RP2040 Racing Steering Wheel Firmware
 * With button hole positions for future soldering
 * OTA support via Mass Storage (recommended)
 */

//========================================
// PIN DEFINITIONS (from PCB design)
//========================================

//=== BUTTONS ===
// 8 buttons with hole positions defined on PCB
// Currently not soldered - wires can be connected to pad locations
#define BUTTON_1    25  // Radio Green - Hole B1
#define BUTTON_2    24  // Radio Red    - Hole B2
#define BUTTON_3    23  // Radio Blue   - Hole B3
#define BUTTON_4    22  // Radio Black  - Hole B4
#define BUTTON_5    21  // Radio Next   - Hole B5
#define BUTTON_6    20  // Radio Back   - Hole B6
#define BUTTON_7    19  // Ecu Red      - Hole B7
#define BUTTON_8    18  // Ecu Yellow   - Hole B8

//=== RESET BUTTON ===
// Connect to EN (GPIO) for manual reset
#define RESET_BUTTON 21  // Hole position: near EN pin

//=== BOOT BUTTON (MASS STORAGE OTA) ===
// Hold during USB plug for mass storage mode
#define BOOT_BUTTON 22  // Hole position: GPIO22

//=== OTHER EXISTING PINS ===
#define LED_STRYPE_PIN      6   // WS2812B DATA
#define I2C_SDA             2   // BH1750 SDA
#define I2C_SCL             3   // BH1750 SCL
#define MENU_ENCODER_A    9   // Encoder A
#define MENU_ENCODER_B    10  // Encoder B
#define MENU_ENCODER_SW   8   // Encoder Switch
#define VOLUME_ENCODER_A  9   // Volume encoder (same pins, different use)
#define VOLUME_ENCODER_B  10
#define VOLUME_ENCODER_SW 8

//=== 3.5mm TRRS JACK (UART to Pedal Box) ===
#define JACK_TX     7   // Tip = UART TX to pedal box
#define JACK_RX     1   // Ring 1 = UART RX from pedal box
#define JACK_GND    GND       // Ring 2 = Signal Ground

//========================================
// BUTTON SETUP (with INPUT_PULLUP)
//========================================

void setupButtons() {
  // All buttons use internal pull-ups
  // When pressed: digitalRead == LOW
  
  pinMode(BUTTON_1, INPUT_PULLUP);  // Radio Green
  pinMode(BUTTON_2, INPUT_PULLUP);  // Radio Red
  pinMode(BUTTON_3, INPUT_PULLUP);  // Radio Blue
  pinMode(BUTTON_4, INPUT_PULLUP);  // Radio Black
  pinMode(BUTTON_5, INPUT_PULLUP);  // Radio Next
  pinMode(BUTTON_6, INPUT_PULLUP);  // Radio Back
  pinMode(BUTTON_7, INPUT_PULLUP);  // Ecu Red
  pinMode(BUTTON_8, INPUT_PULLUP);  // Ecu Yellow
  
  // Reset button (if soldered)
  // pinMode(RESET_BUTTON, INPUT_PULLUP);
  // digitalRead(RESET_BUTTON) == LOW triggers reset
  
  // Boot button (for mass storage OTA)
  // pinMode(BOOT_BUTTON, INPUT_PULLUP);
  // Hold during USB plug for mass storage
}

//========================================
// BUTTON READING FUNCTIONS
//========================================

// Read a specific button (returns true when pressed)
bool buttonPressed(int buttonPin) {
  return digitalRead(buttonPin) == LOW;
}

// Read all 8 buttons
struct ButtonsStatus {
  bool b1_radio_green;
  bool b2_radio_red;
  bool b3_radio_blue;
  bool b4_radio_black;
  bool b5_radio_next;
  bool b6_radio_back;
  bool b7_ecu_red;
  bool b8_ecu_yellow;
};

ButtonsStatus readAllButtons() {
  ButtonsStatus s;
  s.b1_radio_green = buttonPressed(BUTTON_1);
  s.b2_radio_red = buttonPressed(BUTTON_2);
  s.b3_radio_blue = buttonPressed(BUTTON_3);
  s.b4_radio_black = buttonPressed(BUTTON_4);
  s.b5_radio_next = buttonPressed(BUTTON_5);
  s.b6_radio_back = buttonPressed(BUTTON_6);
  s.b7_ecu_red = buttonPressed(BUTTON_7);
  s.b8_ecu_yellow = buttonPressed(BUTTON_8);
  return s;
}

//========================================
// MAIN SETUP
//========================================

void setup() {
  // Initialize serial for UART to pedal box
  Serial.begin(9600);
  
  // Initialize buttons
  setupButtons();
  
  // Initialize I2C for BH1750
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize LED strip
  // strip.begin(); // Uncomment if WS2812B is soldered
  
  // Initialize encoders
  // encoderMenu.setup();
  // encoderVolume.setup();
  
  // Initialize LED matrix (if LCD populated)
  // lcd8Digit.setup();
  
  Serial.println("Racing Wheel Firmware Ready");
  Serial.println("Buttons: Holes defined - wires can be added later");
  Serial.println("OTA: Hold BOOT button + plug USB for mass storage");
}

//========================================
// MAIN LOOP
//========================================

void loop() {
  // Read buttons
  ButtonsStatus buttons = readAllButtons();
  
  // Send button states over UART to pedal box
  if (Serial.availableForWrite()) {
    // Could send button state bytes here
  }
  
  // Read encoder positions
  // encoderMenu.read();
  // encoderVolume.read();
  
  // Update LED strip (if soldered)
  // strip.show();
  
  // Read BH1750 light sensor
  // float lux = bh1750.readLightLevel();
  
  delay(10);
}

//========================================
// OTA FUNCTIONALITY
//========================================

// Mass Storage OTA (RECOMMENDED)
// ================================
// 1. Hold BOOT_BUTTON during USB plug
// 2. RP2040 appears as RPI-RP2040 drive
// 3. Drag and drop .uf2 firmware file
// 4. RP2040 reboots with new firmware

// UART OTA (via 3.5mm TRRS jack)
// ==============================
// 1. Connect pedal box to 3.5mm TRRS jack
// 2. Terminal listens on UART (9600 baud)
// 3. Send .uf2 binary over serial
// 4. Bootloader writes to flash

// Standard USB upload
// ===================
// 1. Hold RESET_BUTTON
// 2. Press BOOT_BUTTON
// 3. Click "Download UF2" in Thonny/VS Code
// 4. Or press BOOTSEL and plug USB

//========================================
// OPTIONAL: Add Buttons Later
// ==========================================
// The PCB has holes and pads at these locations:
// B1-B8: Radio Green-Radio Black (D25-D18)
// Reset: Near EN pin
// Boot: Near GPIO22
//
// To add buttons:
// 1. Solder tactile buttons into the holes
// 2. Connect NO (Normally Open) pin to the defined GPIO
// 2. Connect C (Common) to GND
// 3. The firmware already has INPUT_PULLUP enabled
// 4. Buttons read: digitalRead(pin) == LOW when pressed
//
// Wire example:
//   Button -> GPIO25 (B1/Radio Green)
//   Button -> GPIO24 (B2/Radio Red)
//   ...etc
//   All buttons share GND

/*
========================================================================
FIRMWARE FLASH METHODS

1. MASS STORAGE (EASIEST - RECOMMENDED)
----------------------------------------------------------------
1. Hold BOOT_BUTTON during USB plug
2. Release after RP2040 appears as RPI-RP2040 drive
3. Drag .uf2 firmware file to the drive
4. RP2040 reboots automatically

========================================================================
2. STANDARD USB UPLOAD
----------------------------------------------------------------
1. Hold RESET_BUTTON
2. Press and release BOOT_BUTTON
3. Click "Download UF2" in Thonny IDE or VS Code
4. Or: Press BOOTSEL button, then plug USB cable

========================================================================
3. UART OTA (VIA 3.5mm JACK)
----------------------------------------------------------------
1. Connect pedal box to 3.5mm TRRS jack
2. Use terminal (PuTTY, screen, minicom) at 9600 baud
3. Send .uf2 binary data over serial
4. Bootloader writes to flash chip

========================================================================
4. CIRCUITPY / UF2 Drive (CircuitPython style)
----------------------------------------------------------------
1. Hold BOOT_BUTTON during USB plug
2. Device appears as mass storage
3. Create/uf2 folder with firmware .uf2 files
4. Safely eject and reboot

========================================================================
*/

/*
========================================================================
NEXT STEPS FOR BUTTONS

The PCB is designed with button hole/pad positions, but buttons
are NOT soldered initially. To add buttons:

1. Solder 8 tactile buttons into holes B1-B8 (D25-D18)
2. Connect each button's:
   - NO (Normally Open) -> Defined GPIO pin (see above)
   - C (Common) -> GND
3. The firmware already has INPUT_PULLUP enabled
4. Test: Press button -> digitalRead(pin) == LOW

The board works perfectly without buttons soldered - just:
- Type-C USB for power + OTA
- 3.5mm jack for UART to pedal box
- Encoders for menu/volume
- BH1750 for light sensor
- WS2812B LED (if soldered)

========================================================================
*/
