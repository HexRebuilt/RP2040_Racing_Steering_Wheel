#!/usr sch
# KiCad Schematic Sheet - RP2040 Core
# Description: Raspberry Pi RP2040 microcontroller with all connections
# Date: 2026-08-13
# Author: HexRebuilt

# Sheet settings
(export-version 20240514)
(title "RP2040 Core")
(description "Raspberry Pi RP2040 microcontroller with GPIO, ADC, UART, SPI, I2C")

(
# Component: RP2040 Microcontroller
(def U1
  (part RP2040_Nano_Connect)
  (value "RP2040")
  (everywhere no)
  (mesh bga)
  (region
    (box
      (pt 0 0)
      (pt 150 150)
    )
  )
)
(
# Component: 16MHz Crystal
(def X1
  (part "16MHz_Crystal")
  (value "16MHz")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 200 0)
      (pt 300 50)
    )
  )
)
(
# Component: Load Capacitors
(def C1
  (part "C22pF_X7R")
  (value "22pF")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 300 100)
      (pt 350 150)
    )
  )
)
(
def C2
  (part "C22pF_X7R")
  (value "22pF")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 300 200)
      (pt 350 250)
    )
  )
)
(
# Component: USB Connector
(def USB_D
  (part "USB_Micro_A")
  (value "USB_D+")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 400 0)
      (pt 450 50)
    )
  )
)
(
def USB_D_
  (part "USB_Micro_A")
  (value "USB_D-")
  (everywhere no)
  (mesh pfet)
  (region
    (pt 400 60)
    (pt 450 110)
  )
)

# Power nets
(net 3V3_RP2040
  (netname "3V3_RP2040")
  (show yes)
)
(net GND
  (netname "GND")
  (show yes)
)

# RP2040 pin connections (simplified)
# GPIO definitions
(def GPIO0 (net "GPIO0"))
(def GPIO1 (net "GPIO1"))
(def GPIO2 (net "I2C0_SDA"))
(def GPIO3 (net "I2C0_SCL"))
(def GPIO4 (net "ADC1"))
(def GPIO5 (net "ADC2"))
(def GPIO6 (net "ADC3"))
# ... more GPIOs as needed

# UART0
(def UART0_TX (net "UART0_TX"))
(def UART0_RX (net "UART0_RX"))

# SPI0
(def SPI0_SCK (net "SPI0_SCK"))
(def SPI0_MOSI (net "SPI0_MOSI"))   # COPI
(def SPI0_MISO (net "SPI0_MISO"))  # CIPO

# I2C
(def I2C0_SDA (net "I2C0_SDA"))
(def I2C0_SCL (net "I2C0_SCL"))

# Crystal connections
(Wire
  (net "16MHz_XTAL")
  (pts
    (pt 200 0)
    (pt 200 50)
    (pt 250 50)
    (pt 250 0)
  )
)
(Wire
  (net "16MHz_XTAL")
  (pts
    (pt 300 0)
    (pt 300 50)
    (pt 350 50)
    (pt 350 0)
  )
)
(Wire
  (net "GND")
  (pts
    (pt 0 0)
    (pt 0 50)
    (pt 0 100)
    (pt 0 150)
    (pt 0 200)
  )
)

# Component placements
(XKFig
  (text "RP2040 Core Sheet" (pt 50 260) (font "default") (justify "center") (height 30) (width 60))
  (text "16MHz Crystal" (pt 250 20) (font "default") (justify "left") (height 20) (width 0))
  (text "GPIO0-29 digital" (pt 20 100) (font "default") (justify "left") (height 20) (width 0))
  (text "UART0 TX/RX" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "SPI0 SCK/MOSI/MISO" (pt 20 180) (font "default") (justify "left") (height 20) (width 0))
  (text "I2C0 SDA/SCL" (pt 20 220) (font "default") (justify "left") (height 20) (width 0))
  (text "USB D+/D-" (pt 420 20) (font "default") (justify "left") (height 20) (width 0))
)
)

