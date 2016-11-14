EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:kuro
LIBS:ReceiverE5-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "KBUS Receiver"
Date ""
Rev "2.0"
Comp ""
Comment1 "Generic KBUS receiver for connection to consoles"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LM2731 U2
U 1 1 58245C90
P 4050 1450
F 0 "U2" H 3800 1800 50  0000 C CNN
F 1 "LM2731" H 4200 1800 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 4050 1450 100 0001 C CNN
F 3 "" H 4050 1450 100 0000 C CNN
	1    4050 1450
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 58245D7F
P 5500 1450
F 0 "C6" H 5525 1550 50  0000 L CNN
F 1 "680p" H 5525 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5538 1300 50  0001 C CNN
F 3 "" H 5500 1450 50  0000 C CNN
	1    5500 1450
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D2
U 1 1 58245E7F
P 4900 1250
F 0 "D2" H 4900 1150 50  0000 C CNN
F 1 "MBR0520LT1G" H 4950 1350 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 4900 1250 50  0001 C CNN
F 3 "" H 4900 1250 50  0000 C CNN
	1    4900 1250
	-1   0    0    1   
$EndComp
$Comp
L R R4
U 1 1 58245EDA
P 5200 1450
F 0 "R4" V 5280 1450 50  0000 C CNN
F 1 "47k" V 5200 1450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5130 1450 50  0001 C CNN
F 3 "" H 5200 1450 50  0000 C CNN
	1    5200 1450
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR_SMALL L1
U 1 1 58245F6F
P 4050 800
F 0 "L1" H 4050 900 50  0000 C CNN
F 1 "6.8u" H 4050 750 50  0000 C CNN
F 2 "kuro:SRN4012TA" H 4050 800 50  0001 C CNN
F 3 "" H 4050 800 50  0000 C CNN
	1    4050 800 
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 582463E1
P 5200 1850
F 0 "R5" V 5280 1850 50  0000 C CNN
F 1 "15k" V 5200 1850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5130 1850 50  0001 C CNN
F 3 "" H 5200 1850 50  0000 C CNN
	1    5200 1850
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 58246413
P 5800 1650
F 0 "C7" H 5825 1750 50  0000 L CNN
F 1 "10u" H 5825 1550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5838 1500 50  0001 C CNN
F 3 "" H 5800 1650 50  0000 C CNN
	1    5800 1650
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 58246601
P 3100 1650
F 0 "C3" H 3125 1750 50  0000 L CNN
F 1 "4.7u" H 3125 1550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3138 1500 50  0001 C CNN
F 3 "" H 3100 1650 50  0000 C CNN
	1    3100 1650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR01
U 1 1 582469D6
P 6100 1150
F 0 "#PWR01" H 6100 1000 50  0001 C CNN
F 1 "+5V" H 6100 1290 50  0000 C CNN
F 2 "" H 6100 1150 50  0000 C CNN
F 3 "" H 6100 1150 50  0000 C CNN
	1    6100 1150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 582469FA
P 3100 2150
F 0 "#PWR02" H 3100 1900 50  0001 C CNN
F 1 "GND" H 3100 2000 50  0000 C CNN
F 2 "" H 3100 2150 50  0000 C CNN
F 3 "" H 3100 2150 50  0000 C CNN
	1    3100 2150
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG03
U 1 1 58246A8F
P 10700 1350
F 0 "#FLG03" H 10700 1445 50  0001 C CNN
F 1 "PWR_FLAG" H 10700 1530 50  0000 C CNN
F 2 "" H 10700 1350 50  0000 C CNN
F 3 "" H 10700 1350 50  0000 C CNN
	1    10700 1350
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR04
U 1 1 58246B44
P 10350 1350
F 0 "#PWR04" H 10350 1200 50  0001 C CNN
F 1 "+5V" H 10350 1490 50  0000 C CNN
F 2 "" H 10350 1350 50  0000 C CNN
F 3 "" H 10350 1350 50  0000 C CNN
	1    10350 1350
	1    0    0    -1  
$EndComp
$Comp
L USB_A J1
U 1 1 5824820A
P 950 1500
F 0 "J1" V 650 1600 50  0000 C CNN
F 1 "KBUS" V 650 1400 50  0000 C CNN
F 2 "kuro:USB_A_multicomp" V 900 1400 50  0001 C CNN
F 3 "" V 900 1400 50  0000 C CNN
	1    950  1500
	0    -1   1    0   
$EndComp
$Comp
L +5V #PWR05
U 1 1 5824886A
P 1350 1200
F 0 "#PWR05" H 1350 1050 50  0001 C CNN
F 1 "+5V" H 1350 1340 50  0000 C CNN
F 2 "" H 1350 1200 50  0000 C CNN
F 3 "" H 1350 1200 50  0000 C CNN
	1    1350 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 582489D4
P 1350 1900
F 0 "#PWR06" H 1350 1650 50  0001 C CNN
F 1 "GND" H 1350 1750 50  0000 C CNN
F 2 "" H 1350 1900 50  0000 C CNN
F 3 "" H 1350 1900 50  0000 C CNN
	1    1350 1900
	1    0    0    -1  
$EndComp
$Comp
L TPS782XX U5
U 1 1 5824A115
P 7200 1500
F 0 "U5" H 7200 1850 50  0000 C CNN
F 1 "TPS78233" H 7200 1750 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 7200 1250 50  0001 C CNN
F 3 "" H 7200 1500 60  0000 C CNN
	1    7200 1500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR07
U 1 1 5824ABA7
P 6700 1300
F 0 "#PWR07" H 6700 1150 50  0001 C CNN
F 1 "+5V" H 6700 1440 50  0000 C CNN
F 2 "" H 6700 1300 50  0000 C CNN
F 3 "" H 6700 1300 50  0000 C CNN
	1    6700 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5824AD24
P 6700 1700
F 0 "#PWR08" H 6700 1450 50  0001 C CNN
F 1 "GND" H 6700 1550 50  0000 C CNN
F 2 "" H 6700 1700 50  0000 C CNN
F 3 "" H 6700 1700 50  0000 C CNN
	1    6700 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5824B020
P 7950 1700
F 0 "#PWR09" H 7950 1450 50  0001 C CNN
F 1 "GND" H 7950 1550 50  0000 C CNN
F 2 "" H 7950 1700 50  0000 C CNN
F 3 "" H 7950 1700 50  0000 C CNN
	1    7950 1700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 5824B213
P 7950 1300
F 0 "#PWR010" H 7950 1150 50  0001 C CNN
F 1 "+3.3V" H 7950 1440 50  0000 C CNN
F 2 "" H 7950 1300 50  0000 C CNN
F 3 "" H 7950 1300 50  0000 C CNN
	1    7950 1300
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5824B4EB
P 7800 1500
F 0 "C9" H 7825 1600 50  0000 L CNN
F 1 "10u" H 7825 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7838 1350 50  0001 C CNN
F 3 "" H 7800 1500 50  0000 C CNN
	1    7800 1500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 5824C026
P 10350 1650
F 0 "#PWR011" H 10350 1500 50  0001 C CNN
F 1 "+3.3V" H 10350 1790 50  0000 C CNN
F 2 "" H 10350 1650 50  0000 C CNN
F 3 "" H 10350 1650 50  0000 C CNN
	1    10350 1650
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG012
U 1 1 5824C328
P 10700 1650
F 0 "#FLG012" H 10700 1745 50  0001 C CNN
F 1 "PWR_FLAG" H 10700 1830 50  0000 C CNN
F 2 "" H 10700 1650 50  0000 C CNN
F 3 "" H 10700 1650 50  0000 C CNN
	1    10700 1650
	1    0    0    -1  
$EndComp
$Comp
L ATXMEGA32E5-A U1
U 1 1 5824CC1D
P 3850 4550
F 0 "U1" H 2650 5600 50  0000 L BNN
F 1 "ATXMEGA32E5-A" H 4400 5600 50  0000 L BNN
F 2 "Housings_QFP:TQFP-32_7x7mm_Pitch0.8mm" H 3850 4550 50  0001 C CIN
F 3 "" H 3850 4550 50  0000 C CNN
	1    3850 4550
	1    0    0    -1  
$EndComp
$Comp
L AVR-PDI-6A J2
U 1 1 5824D2A9
P 1650 3750
F 0 "J2" H 1850 3400 50  0000 C CNN
F 1 "AVR-PDI-6A" H 1650 3500 50  0000 C CNN
F 2 "kuro:3x2_1.27mm_SMD_header" V 1100 3800 50  0001 C CNN
F 3 "" H 1650 3750 50  0000 C CNN
	1    1650 3750
	-1   0    0    1   
$EndComp
Text Label 2000 4850 0    50   ~ 0
KBUS_RX
Text Label 2000 4950 0    50   ~ 0
KBUS_TX
$Comp
L SN74LV07A U3
U 1 1 5824E515
P 6850 4050
F 0 "U3" H 6650 4500 50  0000 C CNN
F 1 "SN74LV07A" H 7000 4500 50  0000 C CNN
F 2 "SMD_Packages:SSOP-14" H 6850 4050 100 0001 C CNN
F 3 "" H 6850 4050 100 0000 C CNN
	1    6850 4050
	1    0    0    -1  
$EndComp
$Comp
L SN74LV07A U4
U 1 1 5824E802
P 6850 5000
F 0 "U4" H 6650 5450 50  0000 C CNN
F 1 "SN74LV07A" H 7000 5450 50  0000 C CNN
F 2 "SMD_Packages:SSOP-14" H 6850 5000 100 0001 C CNN
F 3 "" H 6850 5000 100 0000 C CNN
	1    6850 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5824EEE6
P 6350 4400
F 0 "#PWR013" H 6350 4150 50  0001 C CNN
F 1 "GND" H 6350 4250 50  0000 C CNN
F 2 "" H 6350 4400 50  0000 C CNN
F 3 "" H 6350 4400 50  0000 C CNN
	1    6350 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5824F2F5
P 6350 5350
F 0 "#PWR014" H 6350 5100 50  0001 C CNN
F 1 "GND" H 6350 5200 50  0000 C CNN
F 2 "" H 6350 5350 50  0000 C CNN
F 3 "" H 6350 5350 50  0000 C CNN
	1    6350 5350
	1    0    0    -1  
$EndComp
Text Label 5350 4950 2    50   ~ 0
C3
Text Label 5350 5150 2    50   ~ 0
C5
Text Label 5350 5250 2    50   ~ 0
C6
$Comp
L 74LVC2T45 U6
U 1 1 58250100
P 9100 3900
F 0 "U6" H 8900 4200 50  0000 C CNN
F 1 "74LVC2T45" H 9250 4200 50  0000 C CNN
F 2 "kuro:VSSOP8_SOT765-1" H 9100 3900 60  0001 C CNN
F 3 "" H 9100 3900 60  0000 C CNN
	1    9100 3900
	1    0    0    -1  
$EndComp
$Comp
L 74LVC2T45 U7
U 1 1 5825061F
P 9100 4800
F 0 "U7" H 8900 5100 50  0000 C CNN
F 1 "74LVC2T45" H 9250 5100 50  0000 C CNN
F 2 "kuro:VSSOP8_SOT765-1" H 9100 4800 60  0001 C CNN
F 3 "" H 9100 4800 60  0000 C CNN
	1    9100 4800
	1    0    0    -1  
$EndComp
Text Label 8250 4750 0    50   ~ 0
C2
Text Label 8250 4850 0    50   ~ 0
C7
Text Label 8250 3850 0    50   ~ 0
C4
Text Label 8250 3950 0    50   ~ 0
C5
$Comp
L GND #PWR015
U 1 1 58251C44
P 8600 4100
F 0 "#PWR015" H 8600 3850 50  0001 C CNN
F 1 "GND" H 8600 3950 50  0000 C CNN
F 2 "" H 8600 4100 50  0000 C CNN
F 3 "" H 8600 4100 50  0000 C CNN
	1    8600 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 58251C82
P 8600 5000
F 0 "#PWR016" H 8600 4750 50  0001 C CNN
F 1 "GND" H 8600 4850 50  0000 C CNN
F 2 "" H 8600 5000 50  0000 C CNN
F 3 "" H 8600 5000 50  0000 C CNN
	1    8600 5000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 582523F9
P 8600 3700
F 0 "#PWR017" H 8600 3550 50  0001 C CNN
F 1 "+3.3V" H 8600 3840 50  0000 C CNN
F 2 "" H 8600 3700 50  0000 C CNN
F 3 "" H 8600 3700 50  0000 C CNN
	1    8600 3700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 58252575
P 8600 4600
F 0 "#PWR018" H 8600 4450 50  0001 C CNN
F 1 "+3.3V" H 8600 4740 50  0000 C CNN
F 2 "" H 8600 4600 50  0000 C CNN
F 3 "" H 8600 4600 50  0000 C CNN
	1    8600 4600
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR019
U 1 1 58253B37
P 9600 3700
F 0 "#PWR019" H 9600 3550 50  0001 C CNN
F 1 "+5V" H 9600 3840 50  0000 C CNN
F 2 "" H 9600 3700 50  0000 C CNN
F 3 "" H 9600 3700 50  0000 C CNN
	1    9600 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 58254196
P 9600 5000
F 0 "#PWR020" H 9600 4750 50  0001 C CNN
F 1 "GND" H 9600 4850 50  0000 C CNN
F 2 "" H 9600 5000 50  0000 C CNN
F 3 "" H 9600 5000 50  0000 C CNN
	1    9600 5000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR021
U 1 1 582542A3
P 9600 4600
F 0 "#PWR021" H 9600 4450 50  0001 C CNN
F 1 "+5V" H 9600 4740 50  0000 C CNN
F 2 "" H 9600 4600 50  0000 C CNN
F 3 "" H 9600 4600 50  0000 C CNN
	1    9600 4600
	1    0    0    -1  
$EndComp
Text Label 9800 4050 2    50   ~ 0
DIR
Text Label 2000 5350 0    50   ~ 0
DIR
Text Label 5350 4750 2    50   ~ 0
C1
Text Label 5350 4850 2    50   ~ 0
C2
Text Label 5350 5050 2    50   ~ 0
C4
Text Label 5350 5350 2    50   ~ 0
C7
Text Label 5350 3750 2    50   ~ 0
A0
Text Label 5350 4650 2    50   ~ 0
C0
Text Label 5350 4450 2    50   ~ 0
A7
Text Label 5350 4350 2    50   ~ 0
A6
Text Label 5350 4250 2    50   ~ 0
A5
Text Label 5350 4150 2    50   ~ 0
A4
Text Label 5350 4050 2    50   ~ 0
A3
Text Label 5350 3950 2    50   ~ 0
A2
Text Label 5350 3850 2    50   ~ 0
A1
$Comp
L CONN_01X08 J3
U 1 1 582596C0
P 3850 6750
F 0 "J3" H 3850 7200 50  0000 C CNN
F 1 "CONN_01X08" V 3950 6750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08" H 3850 6750 50  0001 C CNN
F 3 "" H 3850 6750 50  0000 C CNN
	1    3850 6750
	1    0    0    -1  
$EndComp
Text Label 3450 6400 0    50   ~ 0
C0
Text Label 3450 6500 0    50   ~ 0
C1
Text Label 3450 6600 0    50   ~ 0
C2
Text Label 3450 6700 0    50   ~ 0
C3
Text Label 3450 6800 0    50   ~ 0
C4
Text Label 3450 6900 0    50   ~ 0
C5
Text Label 3450 7000 0    50   ~ 0
C6
Text Label 3450 7100 0    50   ~ 0
C7
$Comp
L CONN_01X06 J4
U 1 1 58259A6B
P 7600 4000
F 0 "J4" H 7600 4350 50  0000 C CNN
F 1 "CONN_01X06" V 7700 4000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06" H 7600 4000 50  0001 C CNN
F 3 "" H 7600 4000 50  0000 C CNN
	1    7600 4000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X06 J5
U 1 1 5825A06B
P 7600 4950
F 0 "J5" H 7600 5300 50  0000 C CNN
F 1 "CONN_01X06" V 7700 4950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06" H 7600 4950 50  0001 C CNN
F 3 "" H 7600 4950 50  0000 C CNN
	1    7600 4950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR022
U 1 1 5825A872
P 7500 4550
F 0 "#PWR022" H 7500 4400 50  0001 C CNN
F 1 "+3.3V" H 7500 4690 50  0000 C CNN
F 2 "" H 7500 4550 50  0000 C CNN
F 3 "" H 7500 4550 50  0000 C CNN
	1    7500 4550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR023
U 1 1 5825A961
P 7200 5600
F 0 "#PWR023" H 7200 5450 50  0001 C CNN
F 1 "+3.3V" H 7200 5740 50  0000 C CNN
F 2 "" H 7200 5600 50  0000 C CNN
F 3 "" H 7200 5600 50  0000 C CNN
	1    7200 5600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 J6
U 1 1 5825AA3F
P 10650 4000
F 0 "J6" H 10650 4250 50  0000 C CNN
F 1 "CONN_01X04" V 10750 4000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 10650 4000 50  0001 C CNN
F 3 "" H 10650 4000 50  0000 C CNN
	1    10650 4000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 J7
U 1 1 5825BAF0
P 10650 5700
F 0 "J7" H 10650 5950 50  0000 C CNN
F 1 "CONN_01X04" V 10750 5700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 10650 5700 50  0001 C CNN
F 3 "" H 10650 5700 50  0000 C CNN
	1    10650 5700
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR024
U 1 1 5825BCEC
P 10300 5500
F 0 "#PWR024" H 10300 5350 50  0001 C CNN
F 1 "+5V" H 10300 5640 50  0000 C CNN
F 2 "" H 10300 5500 50  0000 C CNN
F 3 "" H 10300 5500 50  0000 C CNN
	1    10300 5500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5825BEA2
P 10300 5900
F 0 "#PWR025" H 10300 5650 50  0001 C CNN
F 1 "GND" H 10300 5750 50  0000 C CNN
F 2 "" H 10300 5900 50  0000 C CNN
F 3 "" H 10300 5900 50  0000 C CNN
	1    10300 5900
	1    0    0    -1  
$EndComp
Text Label 1750 1400 2    50   ~ 0
KBUS_TX
Text Label 1750 1500 2    50   ~ 0
KBUS_RX
$Comp
L Crystal Y1
U 1 1 5825E8B4
P 1100 4450
F 0 "Y1" H 1100 4600 50  0000 C CNN
F 1 "Crystal" H 1100 4300 50  0000 C CNN
F 2 "kuro:SMD_Crystal_Multi_QC6CB_7A_ABM7" H 1100 4450 50  0001 C CNN
F 3 "" H 1100 4450 50  0000 C CNN
	1    1100 4450
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5825EC1D
P 750 4750
F 0 "C1" H 775 4850 50  0000 L CNN
F 1 "22p" H 775 4650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 788 4600 50  0001 C CNN
F 3 "" H 750 4750 50  0000 C CNN
	1    750  4750
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5825EF20
P 1400 4750
F 0 "C2" H 1425 4850 50  0000 L CNN
F 1 "22p" H 1425 4650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1438 4600 50  0001 C CNN
F 3 "" H 1400 4750 50  0000 C CNN
	1    1400 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 5825FD90
P 750 5000
F 0 "#PWR026" H 750 4750 50  0001 C CNN
F 1 "GND" H 750 4850 50  0000 C CNN
F 2 "" H 750 5000 50  0000 C CNN
F 3 "" H 750 5000 50  0000 C CNN
	1    750  5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 582615C7
P 1100 3650
F 0 "#PWR027" H 1100 3400 50  0001 C CNN
F 1 "GND" H 1100 3500 50  0000 C CNN
F 2 "" H 1100 3650 50  0000 C CNN
F 3 "" H 1100 3650 50  0000 C CNN
	1    1100 3650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR028
U 1 1 582618AA
P 900 3800
F 0 "#PWR028" H 900 3650 50  0001 C CNN
F 1 "+3.3V" H 900 3940 50  0000 C CNN
F 2 "" H 900 3800 50  0000 C CNN
F 3 "" H 900 3800 50  0000 C CNN
	1    900  3800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR029
U 1 1 58261F69
P 3600 2800
F 0 "#PWR029" H 3600 2650 50  0001 C CNN
F 1 "+3.3V" H 3600 2940 50  0000 C CNN
F 2 "" H 3600 2800 50  0000 C CNN
F 3 "" H 3600 2800 50  0000 C CNN
	1    3600 2800
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 582623B8
P 3800 3050
F 0 "C4" H 3825 3150 50  0000 L CNN
F 1 "100n" H 3825 2950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3838 2900 50  0001 C CNN
F 3 "" H 3800 3050 50  0000 C CNN
	1    3800 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 582625C0
P 3800 3250
F 0 "#PWR030" H 3800 3000 50  0001 C CNN
F 1 "GND" H 3800 3100 50  0000 C CNN
F 2 "" H 3800 3250 50  0000 C CNN
F 3 "" H 3800 3250 50  0000 C CNN
	1    3800 3250
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 58262824
P 4300 3050
F 0 "C5" H 4325 3150 50  0000 L CNN
F 1 "100n" H 4325 2950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4338 2900 50  0001 C CNN
F 3 "" H 4300 3050 50  0000 C CNN
	1    4300 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 58262CE9
P 4300 3250
F 0 "#PWR031" H 4300 3000 50  0001 C CNN
F 1 "GND" H 4300 3100 50  0000 C CNN
F 2 "" H 4300 3250 50  0000 C CNN
F 3 "" H 4300 3250 50  0000 C CNN
	1    4300 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 58264117
P 3850 5800
F 0 "#PWR032" H 3850 5550 50  0001 C CNN
F 1 "GND" H 3850 5650 50  0000 C CNN
F 2 "" H 3850 5800 50  0000 C CNN
F 3 "" H 3850 5800 50  0000 C CNN
	1    3850 5800
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 582657C3
P 1250 5600
F 0 "D1" H 1250 5700 50  0000 C CNN
F 1 "LED" H 1250 5500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 1250 5600 50  0001 C CNN
F 3 "" H 1250 5600 50  0000 C CNN
	1    1250 5600
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 58265BE1
P 1500 5400
F 0 "R3" V 1580 5400 50  0000 C CNN
F 1 "680" V 1500 5400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1430 5400 50  0001 C CNN
F 3 "" H 1500 5400 50  0000 C CNN
	1    1500 5400
	1    0    0    -1  
$EndComp
Text Label 2000 4650 0    50   ~ 0
OPT1
Text Label 2000 4750 0    50   ~ 0
OPT2
$Comp
L R R1
U 1 1 58266E20
P 1050 6550
F 0 "R1" V 1130 6550 50  0000 C CNN
F 1 "680" V 1050 6550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 980 6550 50  0001 C CNN
F 3 "" H 1050 6550 50  0000 C CNN
	1    1050 6550
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5826702D
P 1300 6550
F 0 "R2" V 1380 6550 50  0000 C CNN
F 1 "680" V 1300 6550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1230 6550 50  0001 C CNN
F 3 "" H 1300 6550 50  0000 C CNN
	1    1300 6550
	1    0    0    -1  
$EndComp
Text Label 800  6300 0    50   ~ 0
OPT1
Text Label 800  6200 0    50   ~ 0
OPT2
$Comp
L GND #PWR033
U 1 1 582674C6
P 1300 6800
F 0 "#PWR033" H 1300 6550 50  0001 C CNN
F 1 "GND" H 1300 6650 50  0000 C CNN
F 2 "" H 1300 6800 50  0000 C CNN
F 3 "" H 1300 6800 50  0000 C CNN
	1    1300 6800
	1    0    0    -1  
$EndComp
Text Label 2600 1250 0    50   ~ 0
BOOST_IN
Wire Wire Line
	3400 1450 3550 1450
Wire Wire Line
	3400 800  3400 1450
Wire Wire Line
	2600 1250 3550 1250
Wire Wire Line
	4550 1250 4750 1250
Wire Wire Line
	5050 1250 6100 1250
Wire Wire Line
	5200 1250 5200 1300
Wire Wire Line
	4550 1650 5500 1650
Wire Wire Line
	5200 1600 5200 1700
Connection ~ 5200 1650
Wire Wire Line
	5500 1250 5500 1300
Connection ~ 5200 1250
Wire Wire Line
	5500 1650 5500 1600
Wire Wire Line
	5200 2050 5200 2000
Wire Wire Line
	3100 2050 6100 2050
Wire Wire Line
	3400 2050 3400 1650
Wire Wire Line
	3400 1650 3550 1650
Connection ~ 5500 1250
Wire Wire Line
	5800 2050 5800 1800
Connection ~ 5200 2050
Wire Wire Line
	3100 1500 3100 1250
Connection ~ 3400 1250
Wire Wire Line
	3100 1800 3100 2150
Connection ~ 3400 2050
Wire Wire Line
	4650 1250 4650 800 
Wire Wire Line
	4650 800  4300 800 
Connection ~ 4650 1250
Wire Wire Line
	3800 800  3400 800 
Connection ~ 3100 2050
Connection ~ 5800 1250
Wire Wire Line
	10350 1350 10350 1400
Wire Wire Line
	10350 1400 10700 1400
Wire Wire Line
	10700 1400 10700 1350
Wire Wire Line
	850  1800 850  1850
Wire Wire Line
	850  1850 1350 1850
Wire Wire Line
	1350 1600 1350 1900
Wire Wire Line
	1250 1600 1350 1600
Connection ~ 1350 1850
Wire Wire Line
	1250 1300 1350 1300
Wire Wire Line
	1350 1300 1350 1200
Wire Wire Line
	6800 1600 6700 1600
Wire Wire Line
	6700 1600 6700 1700
Wire Wire Line
	6800 1400 6700 1400
Wire Wire Line
	6700 1300 6700 1500
Wire Wire Line
	6700 1500 6800 1500
Connection ~ 6700 1400
Wire Wire Line
	7600 1400 7700 1400
Wire Wire Line
	7700 1400 7700 1350
Wire Wire Line
	7700 1350 7950 1350
Wire Wire Line
	7600 1600 7700 1600
Wire Wire Line
	7700 1600 7700 1650
Wire Wire Line
	7700 1650 7950 1650
Connection ~ 7800 1650
Connection ~ 7800 1350
Wire Wire Line
	7950 1350 7950 1300
Wire Wire Line
	7950 1650 7950 1700
Wire Wire Line
	10350 1650 10350 1700
Wire Wire Line
	10350 1700 10700 1700
Wire Wire Line
	10700 1700 10700 1650
Wire Wire Line
	2500 4850 2000 4850
Wire Wire Line
	2000 4950 2500 4950
Wire Wire Line
	5150 3750 6400 3750
Wire Wire Line
	5150 4250 6400 4250
Wire Wire Line
	6400 4350 6350 4350
Wire Wire Line
	6350 4350 6350 4400
Wire Wire Line
	5150 4350 6200 4350
Wire Wire Line
	6200 4350 6200 4700
Wire Wire Line
	6200 4700 6400 4700
Wire Wire Line
	5150 4450 6100 4450
Wire Wire Line
	6100 4450 6100 4800
Wire Wire Line
	6100 4800 6400 4800
Wire Wire Line
	5150 4650 6000 4650
Wire Wire Line
	6000 4650 6000 4900
Wire Wire Line
	6000 4900 6400 4900
Wire Wire Line
	5150 4750 5900 4750
Wire Wire Line
	5900 4750 5900 5000
Wire Wire Line
	5900 5000 6400 5000
Wire Wire Line
	5150 4950 5800 4950
Wire Wire Line
	5800 4950 5800 5100
Wire Wire Line
	5800 5100 6400 5100
Wire Wire Line
	5150 5250 6200 5250
Wire Wire Line
	6200 5250 6200 5200
Wire Wire Line
	6200 5200 6400 5200
Wire Wire Line
	6400 5300 6350 5300
Wire Wire Line
	6350 5300 6350 5350
Wire Wire Line
	5150 3850 6400 3850
Wire Wire Line
	6400 3950 5150 3950
Wire Wire Line
	5150 4050 6400 4050
Wire Wire Line
	6400 4150 5150 4150
Wire Wire Line
	8250 3850 8700 3850
Wire Wire Line
	8700 3950 8250 3950
Wire Wire Line
	8250 4750 8700 4750
Wire Wire Line
	8700 4850 8250 4850
Wire Wire Line
	8600 3700 8600 3750
Wire Wire Line
	8600 3750 8700 3750
Wire Wire Line
	8600 4100 8600 4050
Wire Wire Line
	8600 4050 8700 4050
Wire Wire Line
	9500 3750 9600 3750
Wire Wire Line
	9600 3750 9600 3700
Wire Wire Line
	8600 4600 8600 4650
Wire Wire Line
	8600 4650 8700 4650
Wire Wire Line
	8700 4950 8600 4950
Wire Wire Line
	8600 4950 8600 5000
Wire Wire Line
	9500 4950 9600 4950
Wire Wire Line
	9600 4950 9600 5000
Wire Wire Line
	9500 4650 9600 4650
Wire Wire Line
	9600 4650 9600 4600
Wire Wire Line
	9800 4050 9500 4050
Wire Wire Line
	2000 5350 2500 5350
Wire Wire Line
	5150 4850 5350 4850
Wire Wire Line
	5350 5050 5150 5050
Wire Wire Line
	5150 5150 5350 5150
Wire Wire Line
	5350 5350 5150 5350
Wire Wire Line
	3450 6400 3650 6400
Wire Wire Line
	3650 6500 3450 6500
Wire Wire Line
	3450 6600 3650 6600
Wire Wire Line
	3650 6700 3450 6700
Wire Wire Line
	3450 6800 3650 6800
Wire Wire Line
	3650 6900 3450 6900
Wire Wire Line
	3450 7000 3650 7000
Wire Wire Line
	3450 7100 3650 7100
Wire Wire Line
	7300 3750 7400 3750
Wire Wire Line
	7400 3850 7300 3850
Wire Wire Line
	7300 3950 7400 3950
Wire Wire Line
	7400 4050 7300 4050
Wire Wire Line
	7300 4150 7400 4150
Wire Wire Line
	7400 4250 7300 4250
Wire Wire Line
	7400 4700 7300 4700
Wire Wire Line
	7300 4800 7400 4800
Wire Wire Line
	7400 4900 7300 4900
Wire Wire Line
	7300 5000 7400 5000
Wire Wire Line
	7400 5100 7300 5100
Wire Wire Line
	7300 5200 7400 5200
Wire Wire Line
	9500 3850 10450 3850
Wire Wire Line
	9500 3950 10450 3950
Wire Wire Line
	9500 4750 10250 4750
Wire Wire Line
	10250 4750 10250 4050
Wire Wire Line
	10250 4050 10450 4050
Wire Wire Line
	9500 4850 10350 4850
Wire Wire Line
	10350 4850 10350 4150
Wire Wire Line
	10350 4150 10450 4150
Wire Wire Line
	10450 5550 10300 5550
Wire Wire Line
	10300 5550 10300 5500
Wire Wire Line
	10450 5850 10300 5850
Wire Wire Line
	1250 1400 1750 1400
Wire Wire Line
	1750 1500 1250 1500
Wire Wire Line
	1250 4450 2500 4450
Wire Wire Line
	1400 4450 1400 4600
Wire Wire Line
	950  4450 750  4450
Wire Wire Line
	750  4200 750  4600
Connection ~ 1400 4450
Wire Wire Line
	2500 4350 1400 4350
Wire Wire Line
	1400 4350 1400 4200
Wire Wire Line
	1400 4200 750  4200
Connection ~ 750  4450
Wire Wire Line
	750  4900 750  5000
Wire Wire Line
	750  4950 1400 4950
Wire Wire Line
	1400 4950 1400 4900
Connection ~ 750  4950
Wire Wire Line
	2500 3850 2050 3850
Wire Wire Line
	2500 3750 2150 3750
Wire Wire Line
	2150 3750 2150 3650
Wire Wire Line
	2150 3650 2050 3650
Wire Wire Line
	1250 3850 900  3850
Wire Wire Line
	900  3850 900  3800
Wire Wire Line
	1250 3650 1200 3650
Wire Wire Line
	1200 3650 1200 3600
Wire Wire Line
	1200 3600 1100 3600
Wire Wire Line
	1100 3600 1100 3650
Wire Wire Line
	3600 2800 3600 3400
Wire Wire Line
	3800 2900 3800 2850
Connection ~ 3600 2850
Connection ~ 3800 2850
Wire Wire Line
	4100 3400 4100 2850
Connection ~ 4100 2850
Wire Wire Line
	3800 3250 3800 3200
Wire Wire Line
	4300 3200 4300 3250
Wire Wire Line
	4300 2900 4300 2850
Wire Wire Line
	4300 2850 3600 2850
Wire Wire Line
	3800 5700 3800 5750
Wire Wire Line
	3800 5750 3900 5750
Wire Wire Line
	3900 5750 3900 5700
Wire Wire Line
	3850 5750 3850 5800
Connection ~ 3850 5750
Wire Wire Line
	2500 5150 1500 5150
Wire Wire Line
	1500 5150 1500 5250
Wire Wire Line
	2500 5050 1000 5050
Wire Wire Line
	1000 5050 1000 5600
Wire Wire Line
	1000 5600 1050 5600
Wire Wire Line
	1450 5600 1500 5600
Wire Wire Line
	1500 5600 1500 5550
Wire Wire Line
	2000 4750 2500 4750
Wire Wire Line
	2500 4650 2000 4650
Wire Wire Line
	800  6200 1300 6200
Wire Wire Line
	1300 6200 1300 6400
Wire Wire Line
	800  6300 1050 6300
Wire Wire Line
	1050 6300 1050 6400
Wire Wire Line
	1050 6700 1050 6750
Wire Wire Line
	1050 6750 1300 6750
Wire Wire Line
	1300 6700 1300 6800
Connection ~ 1300 6750
Connection ~ 3100 1250
Wire Wire Line
	10300 5750 10300 5900
Text Label 10000 5650 0    50   ~ 0
BOOST_IN
Wire Wire Line
	10000 5650 10450 5650
Wire Wire Line
	10450 5750 10300 5750
Connection ~ 10300 5850
$Comp
L C C10
U 1 1 582712FE
P 8700 1450
F 0 "C10" H 8725 1550 50  0000 L CNN
F 1 "100n" H 8725 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8738 1300 50  0001 C CNN
F 3 "" H 8700 1450 50  0000 C CNN
	1    8700 1450
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 582722DB
P 9000 1450
F 0 "C11" H 9025 1550 50  0000 L CNN
F 1 "100n" H 9025 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9038 1300 50  0001 C CNN
F 3 "" H 9000 1450 50  0000 C CNN
	1    9000 1450
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 58272359
P 9300 1450
F 0 "C12" H 9325 1550 50  0000 L CNN
F 1 "100n" H 9325 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9338 1300 50  0001 C CNN
F 3 "" H 9300 1450 50  0000 C CNN
	1    9300 1450
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 582723DC
P 9600 1450
F 0 "C13" H 9625 1550 50  0000 L CNN
F 1 "100n" H 9625 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9638 1300 50  0001 C CNN
F 3 "" H 9600 1450 50  0000 C CNN
	1    9600 1450
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 58274E24
P 6100 1650
F 0 "C8" H 6125 1750 50  0000 L CNN
F 1 "10u" H 6125 1550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6138 1500 50  0001 C CNN
F 3 "" H 6100 1650 50  0000 C CNN
	1    6100 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1150 6100 1500
Wire Wire Line
	6100 2050 6100 1800
Connection ~ 5800 2050
Wire Wire Line
	5800 1250 5800 1500
Connection ~ 6100 1250
$Comp
L +3.3V #PWR034
U 1 1 5827EBCB
P 8700 1200
F 0 "#PWR034" H 8700 1050 50  0001 C CNN
F 1 "+3.3V" H 8700 1340 50  0000 C CNN
F 2 "" H 8700 1200 50  0000 C CNN
F 3 "" H 8700 1200 50  0000 C CNN
	1    8700 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1200 8700 1300
Wire Wire Line
	8700 1250 9600 1250
Wire Wire Line
	9000 1250 9000 1300
Connection ~ 8700 1250
Wire Wire Line
	9300 1250 9300 1300
Connection ~ 9000 1250
Wire Wire Line
	9600 1250 9600 1300
Connection ~ 9300 1250
Wire Wire Line
	8700 1600 8700 1700
Wire Wire Line
	8700 1650 9600 1650
Wire Wire Line
	9600 1650 9600 1600
Wire Wire Line
	9300 1600 9300 1650
Connection ~ 9300 1650
Wire Wire Line
	9000 1600 9000 1650
Connection ~ 9000 1650
$Comp
L GND #PWR035
U 1 1 5827F399
P 8700 1700
F 0 "#PWR035" H 8700 1450 50  0001 C CNN
F 1 "GND" H 8700 1550 50  0000 C CNN
F 2 "" H 8700 1700 50  0000 C CNN
F 3 "" H 8700 1700 50  0000 C CNN
	1    8700 1700
	1    0    0    -1  
$EndComp
Connection ~ 8700 1650
$Comp
L R R6
U 1 1 582A684B
P 7500 5450
F 0 "R6" V 7580 5450 50  0000 C CNN
F 1 "2k2" V 7500 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7430 5450 50  0001 C CNN
F 3 "" H 7500 5450 50  0000 C CNN
	1    7500 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 5200 7350 5250
Wire Wire Line
	7350 5250 7500 5250
Connection ~ 7350 5200
Wire Wire Line
	7500 5250 7500 5300
Wire Wire Line
	7300 5300 7350 5300
Wire Wire Line
	7350 5300 7350 5650
Wire Wire Line
	7500 5650 7500 5600
Wire Wire Line
	7200 5650 7500 5650
Wire Wire Line
	7200 5650 7200 5600
Connection ~ 7350 5650
Wire Wire Line
	7500 4550 7350 4550
Wire Wire Line
	7350 4550 7350 4350
Wire Wire Line
	7350 4350 7300 4350
NoConn ~ 2050 3750
NoConn ~ 1250 3750
NoConn ~ 2500 5250
$Comp
L PWR_FLAG #FLG036
U 1 1 582B30DF
P 10700 1950
F 0 "#FLG036" H 10700 2045 50  0001 C CNN
F 1 "PWR_FLAG" H 10700 2130 50  0000 C CNN
F 2 "" H 10700 1950 50  0000 C CNN
F 3 "" H 10700 1950 50  0000 C CNN
	1    10700 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR037
U 1 1 582B3159
P 10700 2000
F 0 "#PWR037" H 10700 1750 50  0001 C CNN
F 1 "GND" H 10700 1850 50  0000 C CNN
F 2 "" H 10700 2000 50  0000 C CNN
F 3 "" H 10700 2000 50  0000 C CNN
	1    10700 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 2000 10700 1950
$EndSCHEMATC
