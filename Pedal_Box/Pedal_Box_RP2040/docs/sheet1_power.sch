#!/usr sch
# KiCad Schematic Sheet - Power Supply
# Description: AMS1117-3.3 LDO, reverse polarity protection, USB power
# Date: 2026-08-13
# Author: HexRebuilt

# Sheet settings
(export-version 20240514)
(title "Power Supply")
(description "AMS1117-3.3 LDO Regulator with reverse polarity protection and USB power input")
(
# Component: AMS1117-3.3 LDO
(unit AMS1117
  (part AMS1117-3.3)
  (value AMS1117-3.3)
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 0 0)
      (pt 100 50)
    )
  )
)
(
# Component: 1N4007 Diode (reverse polarity)
(unit D1
  (part 1N4007)
  (value 1N4007)
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 0 100)
      (pt 50 150)
    )
  )
)
(
# Component: 10µF Input Capacitor
(unit C1
  (part C0G_10uF_25V)
  (value "10uF")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 0 200)
      (pt 50 250)
    )
  )
)
(
# Component: 10µF Output Capacitor
(unit C2
  (part C0G_10uF_25V)
  (value "10uF")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 100 200)
      (pt 150 250)
    )
  )
)
(
# Component: 0.1µF High-frequency Capacitor
(unit C3
  (part C0G_0uF_50V)
  (value "0.1uF")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 200 200)
      (pt 250 250)
    )
  )
)
(
# Component: USB Connector
(unit USB_CON
  (part USB_Micro_A)
  (value "USB Micro-A")
  (everywhere no)
  (mesh pfet)
  (region
    (box
      (pt 300 0)
      (pt 350 50)
    )
  )
)
)

# Nets
(net +12V_IN
  (netname "+12V_IN")
  (show yes)
  (edit text "0" (pt 50 50) (force yes) (hidden no)
    (style "default")
    (units "mil")
  )
)
(net GND
  (netname "GND")
  (show yes)
  (edit text "0" (pt 50 150) (force yes) (hidden no)
    (style "default")
    (units "mil")
  )
)
(net 3V3_RP2040
  (netname "3V3_RP2040")
  (show yes)
  (edit text "0" (pt 50 250) (force yes) (hidden no)
    (style "default")
    (units "mil")
  )
)
(net +5V_USB
  (netname "+5V_USB")
  (show yes)
  (edit text "0" (pt 300 250) (force yes) (hidden no)
    (style "default")
    (units "mil")
  )
)

# Wires and nets
(Wire
  (net +12V_IN)
  (pts
    (pt 0 0)
    (pt 0 50)
  )
)
(Wire
  (net GND)
  (pts
    (pt 0 100)
    (pt 0 150)
    (pt 0 200)
  )
)
(Wire
  (net 3V3_RP2040)
  (pts
    (pt 50 200)
    (pt 50 270)
  )
)
(Wire
  (net +5V_USB)
  (pts
    (pt 300 0)
    (pt 300 50)
  )
)

# Component placements
(XKFig
  (text "AMS1117-3.3 LDO" (pt 25 25) (font "default") (justify "left") (height 20) (width 0))
  (text "1N4007 Polarity Protection" (pt 25 125) (font "default") (justify "left") (height 20) (width 0))
  (text "10uF Input Cap" (pt 25 225) (font "default") (justify "left") (height 20) (width 0))
  (text "10uF Output Cap" (pt 125 225) (font "default") (justify "left") (height 20) (width 0))
  (text "0.1uF HF Cap" (pt 250 225) (font "default") (justify "left") (height 20) (width 0))
  (text "USB Micro-A" (pt 325 25) (font "default") (justify "left") (height 20) (width 0))
  (text "Power Supply Sheet" (pt 50 300) (font "default") (justify "center") (height 30) (width 50))
)
)

