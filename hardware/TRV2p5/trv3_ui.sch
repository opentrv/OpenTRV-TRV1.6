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
LIBS:jack_3n
LIBS:opentrv
LIBS:TRV2p5-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date "2017-04-21"
Rev "1"
Comp "OpenTRV"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L POT RV?
U 1 1 58FA1ED5
P 4600 3500
F 0 "RV?" V 4425 3500 50  0000 C CNN
F 1 "POT" V 4500 3500 50  0000 C CNN
F 2 "" H 4600 3500 50  0001 C CNN
F 3 "" H 4600 3500 50  0001 C CNN
	1    4600 3500
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 58FA211A
P 4350 3750
F 0 "C?" H 4375 3850 50  0000 L CNN
F 1 "C" H 4375 3650 50  0000 L CNN
F 2 "" H 4388 3600 50  0001 C CNN
F 3 "" H 4350 3750 50  0001 C CNN
	1    4350 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 58FA2171
P 4600 4050
F 0 "#PWR?" H 4600 3800 50  0001 C CNN
F 1 "GND" H 4600 3900 50  0000 C CNN
F 2 "" H 4600 4050 50  0001 C CNN
F 3 "" H 4600 4050 50  0001 C CNN
	1    4600 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3500 4450 3500
Wire Wire Line
	4350 3500 4350 3600
Wire Wire Line
	4350 3900 4350 4000
Wire Wire Line
	4350 4000 4600 4000
Wire Wire Line
	4600 3650 4600 4050
Connection ~ 4600 4000
Text HLabel 4250 3250 0    60   Input ~ 0
IO_POWER_UP
Wire Wire Line
	4250 3250 4600 3250
Wire Wire Line
	4600 3250 4600 3350
Text HLabel 4250 3500 0    60   Input ~ 0
POT
Connection ~ 4350 3500
Text Notes 3750 4700 0    60   ~ 0
Cap potentiometer\n- Will probably replace\n  with some kind of\n  mechanical encoder
Text Notes 7300 4900 2    60   ~ 0
Cap Button
$Comp
L micro-switch SW?
U 1 1 58FA2453
P 7100 3850
F 0 "SW?" H 7325 3675 60  0000 C CNN
F 1 "micro-switch" H 7105 3770 60  0000 C CNN
F 2 "" H 7250 3500 60  0001 C CNN
F 3 "" H 7250 3500 60  0001 C CNN
	1    7100 3850
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 58FA24E2
P 7100 3150
F 0 "R?" V 7180 3150 50  0000 C CNN
F 1 "4K7" V 7100 3150 50  0000 C CNN
F 2 "" V 7030 3150 50  0001 C CNN
F 3 "" H 7100 3150 50  0001 C CNN
	1    7100 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 58FA2531
P 7100 4250
F 0 "#PWR?" H 7100 4000 50  0001 C CNN
F 1 "GND" H 7100 4100 50  0000 C CNN
F 2 "" H 7100 4250 50  0001 C CNN
F 3 "" H 7100 4250 50  0001 C CNN
	1    7100 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 4250 7100 4150
Wire Wire Line
	7100 3300 7100 3550
$Comp
L VCC #PWR?
U 1 1 58FA256C
P 7100 2850
F 0 "#PWR?" H 7100 2700 50  0001 C CNN
F 1 "VCC" H 7100 3000 50  0000 C CNN
F 2 "" H 7100 2850 50  0001 C CNN
F 3 "" H 7100 2850 50  0001 C CNN
	1    7100 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 2850 7100 3000
Wire Wire Line
	7100 3400 6650 3400
Connection ~ 7100 3400
Text HLabel 6650 3400 0    60   Input ~ 0
BUTTON_CAP
$Comp
L LED D?
U 1 1 58FB7FD1
P 9300 3800
F 0 "D?" H 9300 3900 50  0000 C CNN
F 1 "LED" H 9300 3700 50  0000 C CNN
F 2 "" H 9300 3800 50  0001 C CNN
F 3 "" H 9300 3800 50  0001 C CNN
	1    9300 3800
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 58FB826E
P 9300 4150
F 0 "R?" V 9380 4150 50  0000 C CNN
F 1 "R" V 9300 4150 50  0000 C CNN
F 2 "" V 9230 4150 50  0001 C CNN
F 3 "" H 9300 4150 50  0001 C CNN
	1    9300 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 3600 9300 3650
Wire Wire Line
	9300 3950 9300 4000
Text HLabel 9200 4400 0    60   Input ~ 0
LED
Wire Wire Line
	9200 4400 9300 4400
Wire Wire Line
	9300 4400 9300 4300
$Comp
L VCC #PWR?
U 1 1 58FB833F
P 9300 3600
F 0 "#PWR?" H 9300 3450 50  0001 C CNN
F 1 "VCC" H 9300 3750 50  0000 C CNN
F 2 "" H 9300 3600 50  0001 C CNN
F 3 "" H 9300 3600 50  0001 C CNN
	1    9300 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
