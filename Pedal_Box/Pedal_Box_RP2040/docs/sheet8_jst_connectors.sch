#!/usr sch
(export-version 20240514)
(title "JST XH Connectors (All Other IO)")
(description "2.5mm pitch headers for digital/analog/valve signals")

# 6-pin male header (digital inputs DIN1-5 + GND)
(def H1 6pin_JST_XH_Male 100 100
  (net DIN1_Opto_Anode (netname "DIN1_Opto"))
  (net DIN2_Opto_Anode (netname "DIN2_Opto"))
  (net DIN3_Opto_Anode (netname "DIN3_Opto"))
  (net DIN4_Opto_Anode (netname "DIN4_Opto"))
  (net DIN5_Opto_Anode (netname "DIN5_Opto"))
  (net GND_Digital (netname "GND_Digital"))

# 4-pin male header (analog inputs Analog1-3 + GND)
(def H2 4pin_JST_XH_Male 300 100
  (net Analog1_Vdiv_Out (netname "Analog1"))
  (net Analog2_Vdiv_Out (netname "Analog2"))
  (net Analog3_Vdiv_Out (netname "Analog3"))
  (net GND_Analog (netname "GND_Analog"))

# 2-pin male header per valve PWM channel
(def H3 2pin_JST_XH_Male_Valve1 500 100
  (net Valve1_PWM_Gate (netname "Valve1_PWM"))
  (net Valve1_PWM_Return (netname "GND"))

(def H4 2pin_JST_XH_Male_Valve2 500 300
  (net Valve2_PWM_Gate (netname "Valve2_PWM"))
  (net Valve2_PWM_Return (netname "GND"))

# 3-pin female headers (for sensor cables)
(def H3p 3pin_JST_XH_Female 700 100
  (net Sensor1_Supply (netname "Sensor1_Supply"))
  (net Sensor1_Signal (netname "Sensor1_Signal"))
  (net Sensor1_GND (netname "Sensor1_GND"))

(def H4p 3pin_JST_XH_Female 700 300
  (net Sensor2_Supply (netname "Sensor2_Supply"))
  (net Sensor2_Signal (netname "Sensor2_Signal"))
  (net Sensor2_GND (netname "Sensor2_GND"))

(def H5p 3pin_JST_XH_Female 700 500
  (net Sensor3_Supply (netname "Sensor3_Supply"))
  (net Sensor3_Signal (netname "Sensor3_Signal"))
  (net Sensor3_GND (netname "Sensor3_GND"))

(XKFig
  (text "JST XH 2.5mm Pitch Connectors" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "1A current rating per pin (sufficient for all signals)" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "Keyed polarization prevents misconnection" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "Cheapest professional connector option" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
  (text "6-pin: Digital optocoupler inputs DIN1-5 + GND" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "4-pin: Analog sensor voltage divider outputs" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
  (text "2-pin: Valve PWM gate control" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
  (text "3-pin: Sensor cables (signal, GND, optional +V)" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
)
)
