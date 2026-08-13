#!/usr sch
(export-version 20240514)
(title "3.5mm Jacks (GPS/Radio/Steering)")
(description "3.5mm TRRS jacks - INTERNAL CABIN ONLY, not for 12V automotive")

# 3 jacks only for: GPS, Radio Out, Steering Wheel
(def J1 3_5mm_TRRS_Jack 100 100 (netname "GPS_JACK"))
(def J2 3_5mm_TRRS_Jack 100 300 (netname "Radio_JACK"))
(def J3 3_5mm_TRRS_Jack 100 500 (netname "Steering_JACK"))

# Jack pin assignments
(nlab GPS_Tip (pt 200 100) (netname "GPS_TX"))
(nlab GPS_Ring (pt 200 200) (netname "GPS_RX"))
(nlab GPS_Sleeve (pt 200 300) (netname "GND"))

(nlab Radio_Tip (pt 200 500) (netname "Radio_Out"))
(nlab Radio_Ring2 (pt 200 550) (netname "Radio_Out2"))
(nlab Radio_Sleeve (pt 200 600) (netname "GND"))

(nlab Steering_Tip (pt 200 800) (netname "Steering_TX"))
(nlab Steering_Ring (pt 200 850) (netname "Steering_RX2"))
(nlab Steering_Sleeve (pt 200 900) (netname "GND"))

# RP2040 UART connections
(nlab UART0_TX_RP2040 (pt 400 100) (netname "UART0_TX"))
(nlab UART0_RX_RP2040 (pt 400 300) (netname "UART0_RX"))

(XKFig
  (text "3.5mm Jacks - INTERNAL CABIN ONLY" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "3 jacks: GPS, Radio, Steering - TRRS 4-pole" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "NOT for 12V automotive signals - cabin wiring only" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "Shield grounded at PCB end only" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
)
)
