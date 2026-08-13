#!/usr sch
(export-version 20240514)
(title "CAN Bus Interface")
(description "MCP2515 CAN controller + TJA1050 automotive transceiver")

# MCP2515 CAN Controller
(def U1 MCP2515 200 100)
(XCLK1 16MHz_Crystal 300 100)
C1 22pF 500 100 (net "16MHz_XTAL")
C2 22pF 500 300 (net "16MHz_XTAL")

# TJA1050 Transceiver
(def U2 TJA1050 400 100)
(NLAB CAN_H (netname "CAN_H"))
(NLAB CAN_L (netname "CAN_L"))
(NLAB GND_CAN (netname "GND_CAN"))

# Termination
(R1 120Ω 700 100 (net "CAN_H"))
(R2 120Ω 700 300 (net "CAN_L"))

# Connector
(XCON1 2pin_Molex_CAN 900 100 (net "CAN_H_Veh") (net "CAN_L_Veh"))

# SPI to RP2040
(NLAB SPI0_SCK_RP2040 (pt 300 100) (netname "SPI0_SCK"))
(NLAB SPI0_MOSI_RP2040 (pt 300 200) (netname "SPI0_MOSI"))
(NLAB SPI0_MISO_RP2040 (pt 300 300) (netname "SPI0_MISO"))
(NLAB SPI0_SS_RP2040 (pt 300 400) (netname "SPI0_SS"))

(XKFig
  (text "CAN Bus Interface" (pt 50 260) (font "default") (justify "center") (height 30) (width 70))
  (text "MCP2515 + TJA1050: industry standard automotive CAN" (pt 20 50) (font "default") (justify "left") (height 20) (width 0))
  (text "Baud rate: 250kbit/s typical" (pt 20 80) (font "default") (justify "left") (height 20) (width 0))
  (text "120Ω termination at PCB end AND vehicle end" (pt 20 110) (font "default") (justify "left") (height 20) (width 0))
  (text "CAN_H and CAN_L to vehicle diagnostic connector" (pt 20 140) (font "default") (justify "left") (height 20) (width 0))
  (text "SPI signals to RP2040: SCK, MOSI, MISO, CS" (pt 20 170) (font "default") (justify "left") (height 20) (width 0))
)
)
