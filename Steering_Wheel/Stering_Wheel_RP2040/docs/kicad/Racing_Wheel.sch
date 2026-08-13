(schsheet "Racing Wheel PCB"
  (view 0 0 210 297)
  (titleblock
    (title "RP2040 Racing Steering Wheel")
    (project "Racing_Wheel")
    (sheet "1")
    (date "2026-08-13")
    (company "Racing Projects")
    (at "0" "0")
    (size "A4"
      (unit "mm")
    )
  )
  (libpart "RP2040" "U1"
    (at 100 100)
    (status "working")
    (description "RP2040 Microcontroller")
    (pin
      (name "GPIO0" "D0")
      (number "1")
      (type "input")
    )
    (pin
      (name "GPIO1" "D1")
      (number "2")
      (type "input")
    )
    (pin
      (name "GPIO2" "D2" "BH1750 SDA")
      (number "3")
      (type "input_output")
    )
    (pin
      (name "GPIO3" "D3" "BH1750 SCL")
      (number "4")
      (type "input_output")
    )
    (pin
      (name "GPIO4" "D4")
      (number "5")
      (type "input_output")
    )
    (pin
      (name "GPIO5" "D5")
      (number "6")
      (type "input_output")
    )
    (pin
      (name "GPIO6" "D6" "WS2812B DATA")
      (number "7")
      (type "input_output")
    )
    (pin
      (name "GPIO7" "D7" "MENU_PRESS")
      (number "8")
      (type "input_output")
    )
    (pin
      (name "GPIO8" "D8" "VOLUME_PRESS")
      (number "9")
      (type "input_output")
    )
    (pin
      (name "GPIO9" "D9" "ENCODER_MENU_A")
      (number "10")
      (type "input_output")
    )
    (pin
      (name "GPIO10" "D10" "ENCODER_VOLUME_A")
      (number "11")
      (type "input_output")
    )
    (pin
      (name "GPIO11" "D11" "MENU_COUNT")
      (number "12")
      (type "input_output")
    )
    (pin
      (name "GPIO12" "D12" "MENU_UP")
      (number "13")
      (type "input_output")
    )
    (pin
      (name "GPIO13" "D13" "RADIO_BLUE")
      (number "14")
      (type "input_output")
    )
    (pin
      (name "GPIO14" "D14" "RADIO_GREEN")
      (number "15")
      (type "input_output")
    )
    (pin
      (name "GPIO15" "D15" "RADIO_RED")
      (number "16")
      (type "input_output")
    )
    (pin
      (name "GPIO16" "D16" "ECU_WHITE")
      (number "17")
      (type "input_output")
    )
    (pin
      (name "GPIO17" "D17" "ECU_YELLOW")
      (number "18")
      (type "input_output")
    )
    (pin
      (name "GPIO18" "D18" "ECU_RED")
      (number "19")
      (type "input_output")
    )
    (pin
      (name "GPIO19" "D19" "RADIO_BACK")
      (number "20")
      (type "input_output")
    )
    (pin
      (name "GPIO20" "D20" "RADIO_NEXT")
      (number "21")
      (type "input_output")
    )
    (pin
      (name "GPIO21" "D21" "RADIO_BLACK")
      (number "22")
      (type "input_output")
    )
    (pin
      (name "GPIO22" "D22" "RADIO_BLUE")
      (number "23")
      (type "input_output")
    )
    (pin
      (name "GPIO23" "D23" "RADIO_RED")
      (number "24")
      (type "input_output")
    )
    (pin
      (name "GPIO24" "D24" "RADIO_GREEN")
      (number "25")
      (type "input_output")
    )
    (pin
      (name "GPIO25" "D25" "RADIO_GREEN")
      (number "26")
      (type "input_output")
    )
    (pin
      (name "GPIO26..28" "NC")
      (number "27-28")
      (type "power")
    )
    (pin
      (name "3V3" "3.3V_POWER")
      (number "34")
      (type "power_output")
    )
    (pin
      (name "5V" "5V_POWER")
      (number "35")
      (type "power_input")
    )
    (pin
      (name "GND" "GROUND")
      (number "36,39,40")
      (type "power_input")
    )
  )
  (libpart "Connector_USB-C" "U2"
    (at 300 100)
    (status "working")
    (description "USB Type-C Receptacle")
    (pin
      (name "VBUS" "5V_POWER")
      (number "1")
      (type "power_input")
    )
    (pin
      (name "D-" "D-")
      (number "2")
      (type "signal")
    )
    (pin
      (name "D+" "D+")
      (number "3")
      (type "signal")
    )
    (pin
      (name "ID" "ID")
      (number "4")
      (type "signal")
    )
    (pin
      (name "GND" "GROUND")
      (number "5")
      (type "power_input")
    )
  )
  (libpart "Jack_3.5mm_TRRS" "J1"
    (at 500 100)
    (status "working")
    (description "3.5mm 4-Pole Jack for UART to Pedal Box")
    (pin
      (name "Tip" "UART_TX" "To Pedal Box TX")
      (number "1")
      (type "signal_output")
    )
    (pin
      (name "Ring1" "UART_RX" "From Pedal Box RX")
      (number "2")
      (type "signal_input")
    )
    (pin
      (name "Ring2" "GND" "Signal Ground")
      (number "3")
      (type "power_input")
    )
    (pin
      (name "Sleeve" "NC" "Not Connected")
      (number "4")
      (type "signal")
    )
  )
  (libpart "Encoder_KY040" "ENC1"
    (at 200 300)
    (status "working")
    (description "KY-040 Rotary Encoder - Menu")
    (pin
      (name "A" "GPIO9" "D9" "Encoder A")
      (number "1")
      (type "input")
    )
    (pin
      (name "B" "GPIO10" "D10" "Encoder B")
      (number "2")
      (type "input")
    )
    (pin
      (name "Switch" "GPIO8" "D8" "Encoder Switch")
      (number "3")
      (type "input")
    )
  )
  (libpart "Encoder_KY040" "ENC2"
    (at 200 350)
    (status "working")
    (description "KY-040 Rotary Encoder - Volume")
    (pin
      (name "A" "GPIO9" "D9" "Encoder A")
      (number "1")
      (type "input")
    )
    (pin
      (name "B" "GPIO10" "D10" "Encoder B")
      (number "2")
      (type "input")
    )
    (pin
      (name "Switch" "GPIO8" "D8" "Encoder Switch")
      (number "3")
      (type "input")
    )
  )
  (libpart "Button_Tactile" "B1"
    (at 100 200)
    (status "working")
    (description "Tactile Button - Radio Green")
    (pin
      (name "NO" "GPIO25" "D25" "Radio Green")
      (number "1")
      (type "input")
    )
    (pin
      (name "C" "GND")
      (number "2")
      (type "power_input")
    )
  )
  (libpart "Button_Tactile" "B2"
    (at 100 250)
    (status "working")
    (description "Tactile Button - Radio Red")
    (pin
      (name "NO" "GPIO24" "D24" "Radio Red")
      (number "1")
      (type "input")
    )
    (pin
      (name "C" "GND")
      (number "2")
      (type "power_input")
    )
  )
  (libpart "BH1750" "U3"
    (at 400 300)
    (status "working")
    (description "BH1750 I2C Light Sensor")
    (pin
      (name "VCC" "3V3_POWER")
      (number "1")
      (type "power_input")
    )
    (pin
      (name "GND" "GROUND")
      (number "2")
      (type "power_input")
    )
    (pin
      (name "SDA" "GPIO2" "D2" "I2C Data")
      (number "3")
      (type "input_output")
    )
    (pin
      (name "SCL" "GPIO3" "D3" "I2C Clock")
      (number "4")
      (type "input_output")
    )
  )
  (libpart "LedStrip_WS2812B" "LED1"
    (at 400 200)
    (status "working")
    (description "WS2812B LED Strip")
    (pin
      (name "DATA" "GPIO6" "D6" "LED Strip Data")
      (number "1")
      (type "signal_output")
    )
    (pin
      (name "VCC" "5V_POWER")
      (number "2")
      (type "power_input")
    )
    (pin
      (name "GND" "GROUND")
      (number "3")
      (type "power_input")
    )
    (pin
      (name "NC" "NC")
      (number "4")
      (type "no_connect")
    )
  )
)EOF
echo "Created schematic"
# ============================================
# RP2040 Reset Button
# ============================================
(Symbol "Reset_Button"
  (prefix SW1)
  (description "RP2040 Reset Button")
  (at 100 350)
  
  # Connects EN to GND when pressed
  (pin
    (name "RESET" "EN" "input")
    (number "1")
    (type "input")
  )
  (pin
    (name "GND" "GROUND" "power")
    (number "2")
    (type "power")
  )
)

# ============================================
# RP2040 Boot Button (BOOTSEL)
# ============================================
(Symbol "Boot_Button"
  (prefix SW2)
  (description "RP2040 BootSEL Button - holds during USB plug for mass storage")
  (at 100 400)
  
  # Connects GPIO22 to GND (or GPIO0 behavior)
  (pin
    (name "BOOT" "GPIO22" "input")
    (number "1")
    (type "input")
  )
  (pin
    (name "GND" "GROUND" "power")
    (number "2")
    (type "power")
  )
)

# ============================================
# OTA Capability Notes
# ============================================
(Symbol "OTA_Info"
  (prefix OTAG)
  (description "OTA Methods Supported:
  1. Mass Storage (BOOTSEL + USB - drag .uf2)
  2. UART OTA (via 3.5mm jack)
  3. Internal bootloader checks UART on startup")
  (at 100 450)
)
