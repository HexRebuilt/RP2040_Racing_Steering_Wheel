#!/usr sch
(export-version 20240514)
(title "MOSFET Valve Drivers")
(description "N-channel MOSFET PWM drivers for valve control with freewheeling diodes")

# 4 valve channels (use 2-4 as needed)
(def M1 AO3401 100 100)
(def M2 AO3401 100 300)
(def M3 BSS123 100 500)
(def M4 BSS123 100 700

# Gate resistors
(def R1 1kΩ_Valve1_Gate 200 100)
(def R2 1kΩ_Valve2_Gate 200 300)
(def R3 1kΩ_Valve3_Gate 200 500)
(def R4 1kΩ_Valve4_Gate 200 700

# Freewheeling diodes
(def D1 1N4007_Valve1_Cathode 300 100)
(def D2 1N4007_Valve2_Cathode 300 300)
(def D3 1N4007_Valve3_Cathode 300 500)
(def D4 1N4007_Valve4_Cathode 300 700

# Valve coils (connect to +12V_Molex at drain)
# MOSFET source → GND

# Net labels
(nlab Valve1_Gate (pt 250 100) (netname "Valve1_PWM"))
(nlab Valve2_Gate (pt 250 300) (netname "Valve2_PWM"))
(nlab Valve3_Gate (pt 250 500) (netname "Valve3_PWM"))
(nlab Valve4_Gate (pt 250 700) (netname "Valve4_PWM"))
(nlab +12V_Molex (pt 400 100) (netname "+12V"))
(nlab GND (pt 400 700) (netname "GND"))

# PWM control labels
(nlab Valve1_PWM_Src (pt 500 100) (netname "RP2040_GPIO"))
(nlab Valve2_PWM_Src (pt 500 300) (netname "RP2040_GPIO"))
(nlab Valve3_PWM_Src (pt 500 500) (netname "RP2040_GPIO"))
(nlab Valve4_PWM_Src (pt 500 700) (netname "RP2040_GPIO"))

(XKFig
  (text "MOSFET Valve Drivers" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "N-channel MOSFET: AO3401 / BSS123, Vgs(th) < 3.3V" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "1kΩ gate resistor limits EMI, protects RP2040 GPIO" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "PWM frequency: 100Hz-1kHz (valve mechanical time constant)" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
  (text "Duty cycle: 0-100% partial voltage control" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "Freewheeling diode: 1N4007, cathode to +12V, anode to coil" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
  (text "Per-channel current: 200-500mA typical valve coil" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
)
)
