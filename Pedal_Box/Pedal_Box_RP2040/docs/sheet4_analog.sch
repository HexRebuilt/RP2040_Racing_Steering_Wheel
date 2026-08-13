#!/usr sch
# KiCad Schematic Sheet: Analog ADC Inputs (5ch)
# Description: Voltage dividers for 0-5V automotive sensors
# Date: 2026-08-13

(export-version 20240514)
(title "Analog ADC Inputs")
(description "5 analog ADC inputs for raw automotive sensor data (temperature, pressure, voltage)")

(
# 5 analog channels with voltage dividers
(def A1 Voltage_Div_1 100 100)
(def A2 Voltage_Div_2 100 200)
(def A3 Voltage_Div_3 100 300)
(def A4 Voltage_Div_4 100 400)
(def A5 Voltage_Div_5 100 500

# Top resistors (20kΩ) - sensor side
(def R1 20kΩ_Sensor1 50 100)
(def R2 20kΩ_Sensor2 50 200)
(def R3 20kΩ_Sensor3 50 300)
(def R4 20kΩ_Sensor4 50 400)
(def R5 20kΩ_Sensor5 50 500

# Bottom resistors (10kΩ) - ADC side
(def R3b 10kΩ_ADC1 150 100)
(def R4b 10kΩ_ADC2 150 200)
(def R5b 10kΩ_ADC3 150 300)
(def R6b 10kΩ_ADC4 150 400)
(def R7b 10kΩ_ADC5 150 500

# TVS Diodes (ESD protection, optional but recommended)
(def TVS1 SP6A_ESD 100 250)
(def TVS2 SP6A_ESD 100 350)
(def TVS3 SP6A_ESD 100 450)
(def TVS4 SP6A_ESD 100 550)
(def TVS5 SP6A_ESD 100 650

# capacitors for noise filtering
(def C1 0.1uF_Filter 150 250)
(def C2 0.1uF_Filter 150 350)
(def C3 0.1uF_Filter 150 450)
(def C4 0.1uF_Filter 150 550)
(def C5 0.1uF_Filter 150 650

# Net labels for analog inputs
(nlab Sensor1_Input (pt 200 100) (netname "Sensor1"))
(nlab Sensor2_Input (pt 200 200) (netname "Sensor2"))
(nlab Sensor3_Input (pt 200 300) (netname "Sensor3"))
(nlab Sensor4_Input (pt 200 400) (netname "Sensor4"))
(nlab Sensor5_Input (pt 200 500) (netname "Sensor5"))

(nlab GND_Analog (pt 200 700) (netname "GND_ANALOG"))

(nlab ADC0 (pt 300 100) (netname "ADC0"))
(nlab ADC1 (pt 300 200) (netname "ADC1"))
(nlab ADC2 (pt 300 300) (netname "ADC2"))
(nlab ADC3 (pt 300 400) (netname "ADC3"))
(nlab ADC5 (pt 300 500) (netname "ADC5"))  # ADC5 if using available GPIO

# Connections - Voltage divider circuits
# Each channel: Sensor 0-5V → 20kΩ → Node A → 10kΩ → Node B → RP2040 ADC
#              TVS diode from Node A to GND (optional, ESD protection)
#              0.1µF capacitor from Node B to GND (noise filtering)

# Sensor 1 circuit
(Wire Sensor1_Input (pts (pt 0 100) (pt 50 100)))
(Wire GND_Analog (pts (pt 0 100) (pt 0 150) (pt 0 200)))

# Voltage divider circuit for each channel
# Sensor1: 20kΩ from Sensor1_Input to Node1, 10kΩ from Node1 to ADC0
# (Net names connected below)

# TVS diodes from sensor nodes to GND
(Unit TVS1
  (net Sensor1_Input SP6A_cathode1)
  (net GND_Analog SP6A_anode1)
)

(Unit TVS2
  (net Sensor2_Input SP6A_cathode2)
  (net GND_Analog SP6A_anode2)
)

(Unit TVS3
  (net Sensor3_Input SP6A_cathode3)
  (net GND_Analog SP6A_anode3)
)

(Unit TVS4
  (net Sensor4_Input SP6A_cathode4)
  (net GND_Analog SP6A_anode4)
)

(Unit TVS5
  (net Sensor5_Input SP6A_cathode5)
  (net GND_Analog SP6A_anode5)
)

# Capacitors from ADC side to GND
(Unit C1
  (net ADC0 0.1uF_Filter_gnd1)
  (net GND_Analog 0.1uF_Filter_gnd_common1)
)

(Unit C2
  (net ADC1 0.1uF_Filter_gnd2)
  (net GND_Analog 0.1uF_Filter_gnd_common2)
)

(Unit C3
  (net ADC2 0.1uF_Filter_gnd3)
  (net GND_Analog 0.1uF_Filter_gnd_common3)
)

(Unit C4
  (net ADC3 0.1uF_Filter_gnd4)
  (net GND_Analog 0.1uF_Filter_gnd_common4)
)

(Unit C5
  (net ADC5 0.1uF_Filter_gnd5)
  (net GND_Analog 0.1uF_Filter_gnd_common5)
)

# RP2040 ADC connections (from Sheet 2)
(nlab ADC0_RP2040 (pt 400 100) (netname "ADC0"))
(nlab ADC1_RP2040 (pt 400 200) (netname "ADC1"))
(nlab ADC2_RP2040 (pt 400 300) (netname "ADC2"))
(nlab ADC3_RP2040 (pt 400 400) (netname "ADC3"))

# Verification notes
(XKFig
  (text "5 Analog ADC Inputs" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "Voltage divider: 20kΩ + 10kΩ scales 0-5V → 0-1.67V" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "Safe: max 1.67V from 5V sensor, well within 3.3V ADC limit" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "TVS diodes protect against transient spikes (>5.5V)" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
  (text "ADC 12-bit resolution: 3.3V/4096 ≈ 0.8mV" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "Even 5.5V transient → 5.5V×10/30 = 1.83V (still safe)" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
)
)

