#!/usr sch
(export-version 20240514)
(title "Optocoupler-Isolated Digital Inputs")
(description "5 optocoupler-isolated digital inputs for 12V automotive signals")

(
# 5 optocoupler channels
(def U1 PC817_DIN1 100 100)
(def U2 PC817_DIN2 100 200)
(def U3 PC817_DIN3 100 300)
(def U4 PC817_DIN4 100 400)
(def U5 PC817_DIN5 100 500

# 470Ω current-limiting resistors
(def R1 470Ω_DIN1 50 50)
(def R2 470Ω_DIN2 50 250)
(def R3 470Ω_DIN3 50 350)
(def R4 470Ω_DIN4 50 450)
(def R5 470Ω_DIN5 50 550

# 10kΩ pull-down resistors
(def R6 10kΩ_GPIO0 150 100)
(def R7 10kΩ_GPIO1 150 200)
(def R8 10kΩ_GPIO2 150 300)
(def R9 10kΩ_GPIO3 150 400)
(def R10 10kΩ_GPIO4 150 500

# Net labels
(nlab DIN1_Input (pt 200 100) (netname "DIN1"))
(nlab DIN2_Input (pt 200 200) (netname "DIN2"))
(nlab DIN3_Input (pt 200 300) (netname "DIN3"))
(nlab DIN4_Input (pt 200 400) (netname "DIN5"))
(nlab GND_Digital (pt 200 600) (netname "GND"))

# Connections
(Wire DIN1_Input (pts (pt 0 100) (pt 50 100)))
(Wire GND_Digital (pts (pt 0 100) (pt 0 150)))

# PC817 units
(Unit U1 (net DIN1_Input PC817_anode1) (net GND_Digital PC817_cathode1) (net GND_Digital PC817_pull_down1))
(Unit U2 (net DIN2_Input PC817_anode2) (net GND_Digital PC817_cathode2) (net GND_Digital PC817_pull_down2))
(Unit U3 (net DIN3_Input PC817_anode3) (net GND_Digital PC817_cathode3) (net GND_Digital PC817_pull_down3))
(Unit U4 (net DIN4_Input PC817_anode4) (net GND_Digital PC817_cathode4) (net GND_Digital PC817_pull_down4))
(Unit U5 (net DIN5_Input PC817_anode5) (net GND_Digital PC817_cathode5) (net GND_Digital PC817_pull_down5))

# GPIO labels
(nlab GPIO_DIN1 (pt 300 100) (netname "GPIO0"))
(nlab GPIO_DIN2 (pt 300 200) (netname "GPIO1"))
(nlab GPIO_DIN3 (pt 300 300) (netname "GPIO2"))
(nlab GPIO_DIN4 (pt 300 400) (netname "GPIO3"))
(nlab GPIO_DIN5 (pt 300 500) (netname "GPIO4"))

(XKFig
  (text "5 Isolated Digital Inputs" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "12V → 470Ω → PC817 → GPIO with 10k pull-down" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "12V present = GPIO LOW (with INPUT_PULLUP)" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "12V absent = GPIO HIGH" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
  (text "Max current: 12V/470Ω ≈ 25mA per channel" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "Isolation: >1000V RMS between car and RP2040" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
)
)
