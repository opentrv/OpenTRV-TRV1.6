/*
The OpenTRV project licenses this file to you
under the Apache Licence, Version 2.0 (the "Licence");
you may not use this file except in compliance
with the Licence. You may obtain a copy of the Licence at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the Licence is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied. See the Licence for the
specific language governing permissions and limitations
under the Licence.

Author(s) / Copyright (s): Deniz Erbilgin 2017
*/
/**
 * Minimal REV7 config for testing power consumption.
 * Aim is to:
 *     - todo init GPIO pins to safe mode.
 *     - todo init peripherals to safe low power mode.
 *     - loop endlessly.
 */

// INCLUDES & DEFINES
// Debug output flag
#define DEBUG
// REV7 / DORM1 all-in-one valve unit, secure TX.
#define CONFIG_TRV20_PROTO
// Get defaults for valve applications.
#include <OTV0p2_valve_ENABLE_defaults.h>
// All-in-one valve unit (DORM1).
#include <OTV0p2_CONFIG_REV20.h>
// I/O pin allocation and setup: include ahead of I/O module headers.
#include <OTV0p2_Board_IO_Config.h>
// OTV0p2Base Libraries
#include <OTV0p2Base.h>
// RadValve libraries
#include <OTRadValve.h>


// Debugging output
#ifndef DEBUG
#define DEBUG_SERIAL_PRINT(s) // Do nothing.
#define DEBUG_SERIAL_PRINTFMT(s, format) // Do nothing.
#define DEBUG_SERIAL_PRINT_FLASHSTRING(fs) // Do nothing.
#define DEBUG_SERIAL_PRINTLN_FLASHSTRING(fs) // Do nothing.
#define DEBUG_SERIAL_PRINTLN() // Do nothing.
#define DEBUG_SERIAL_TIMESTAMP() // Do nothing.
#else
// Send simple string or numeric to serial port and wait for it to have been sent.
// Make sure that Serial.begin() has been invoked, etc.
#define DEBUG_SERIAL_PRINT(s) { OTV0P2BASE::serialPrintAndFlush(s); }
#define DEBUG_SERIAL_PRINTFMT(s, fmt) { OTV0P2BASE::serialPrintAndFlush((s), (fmt)); }
#define DEBUG_SERIAL_PRINT_FLASHSTRING(fs) { OTV0P2BASE::serialPrintAndFlush(F(fs)); }
#define DEBUG_SERIAL_PRINTLN_FLASHSTRING(fs) { OTV0P2BASE::serialPrintlnAndFlush(F(fs)); }
#define DEBUG_SERIAL_PRINTLN() { OTV0P2BASE::serialPrintlnAndFlush(); }
// Print timestamp with no newline in format: MinutesSinceMidnight:Seconds:SubCycleTime
extern void _debug_serial_timestamp();
#define DEBUG_SERIAL_TIMESTAMP() _debug_serial_timestamp()
#endif // DEBUG

// OBJECTS & VARIABLES
/**
 * Peripherals present on REV7
 * - Pot        Set input to Hi-Z?
 * - PHT        IO_POWER_UP LOW
 * - Encoder    IO_POWER_UP LOW
 * - LED        Set pin HIGH?
 * - Button     Set to INPUT
 * - H-Bridge   Control pins HIGH, Current sense set to input.
 * - TMP112     read() once.
 * - XTAL       Setup and leave running?
 * - UART       Disable
 */

/**
 * Dummy Types
 */
// Placeholder class with dummy static status methods to reduce code complexity.
typedef OTV0P2BASE::DummySensorOccupancyTracker OccupancyTracker;

/**
 * Supply Voltage instance
 */
// Sensor for supply (eg battery) voltage in millivolts.
// Singleton implementation/instance.
OTV0P2BASE::SupplyVoltageCentiVolts Supply_cV;

/*
 * TMP112 instance
 */
OTV0P2BASE::RoomTemperatureC16_TMP112 TemperatureC16;

/**
 * Temp pot
 */
// Sensor for temperature potentiometer/dial UI control.
// Correct for DORM1/TRV1 with embedded REV7.
// REV7 does not drive pot from IO_POWER_UP.
typedef OTV0P2BASE::SensorTemperaturePot<OccupancyTracker, nullptr, 48, 296, false> TempPot_t;
TempPot_t TempPot;

/**
 * Ambient Light Sensor
 */
typedef OTV0P2BASE::SensorAmbientLight AmbientLight;
// Singleton implementation/instance.
AmbientLight AmbLight;

/**
 * Valve Actuator
 */
// DORM1/REV7 direct drive motor actuator.
static constexpr bool binaryOnlyValveControl = false;
static constexpr uint8_t m1 = MOTOR_DRIVE_ML;
static constexpr uint8_t m2 = MOTOR_DRIVE_MR;
static constexpr uint8_t motorNSleep = 0; // FIXME!!!!
//typedef OTRadValve::DRV8850HardwareDriver<m1, m2, motorSleep, MOTOR_DRIVE_MI_AIN, MOTOR_DRIVE_MC_AIN, decltype(Supply_cV), &Supply_cV, binaryOnlyValveControl> ValveDirect_t; // XXX using alternative driver
//// Singleton implementation/instance.
//// Suppress unnecessary activity when room dark, eg to avoid disturbance if device crashes/restarts,
//// unless recent UI use because value is being fitted/adjusted.
//ValveDirect_t ValveDirect([](){return(AmbLight.isRoomDark());});
OTRadValve::DRV8850HardwareDriver<m1, m2, motorNSleep, 1, MOTOR_DRIVE_MC_AIN> motorDriver;
//OTRadValve::ValveMotorDirectV1HardwareDriver<motorLeft, motorRight, motorCurrent, motorEncoder> motorDriver;
OTRadValve::TestValveMotor ValveDirect((OTRadValve::HardwareMotorDriverInterface *)(&motorDriver));

// FUNCTIONS

/**
 * Panic Functions
 */
// Indicate that the system is broken in an obvious way (distress flashing the main LED).
// DOES NOT RETURN.
// Tries to turn off most stuff safely that will benefit from doing so, but nothing too complex.
// Tries not to use lots of energy so as to keep distress beacon running for a while.
void panic()
{
    // Reset radio and go into low-power mode.
//    PrimaryRadio.panicShutdown();
    // Power down almost everything else...
    OTV0P2BASE::minimisePowerWithoutSleep();
    pinMode(OTV0P2BASE::LED_HEATCALL_L, OUTPUT);
    for( ; ; )
    {
        OTV0P2BASE::LED_HEATCALL_ON();
        OTV0P2BASE::nap(WDTO_15MS);
        OTV0P2BASE::LED_HEATCALL_OFF();
        OTV0P2BASE::nap(WDTO_120MS);
    }
}
// Panic with fixed message.
void panic(const __FlashStringHelper *s)
{
    OTV0P2BASE::serialPrintlnAndFlush(); // Start new line to highlight error.  // May fail.
    OTV0P2BASE::serialPrintAndFlush('!'); // Indicate error with leading '!' // May fail.
    OTV0P2BASE::serialPrintlnAndFlush(s); // Print supplied detail text. // May fail.
    panic();
}


/**
 * @brief   Set pins and on-board peripherals to safe low power state.
 */


//========================================
// SETUP
//========================================

// Setup routine: runs once after reset.
// Does some limited board self-test and will panic() if anything is obviously broken.
void setup()
{
    // Set appropriate low-power states, interrupts, etc, ASAP.
    OTV0P2BASE::powerSetup();
    // IO setup for safety, and to avoid pins floating.
    OTV0P2BASE::IOSetup();

    OTV0P2BASE::serialPrintAndFlush(F("\r\nOpenTRV: ")); // Leading CRLF to clear leading junk, eg from bootloader.
    V0p2Base_serialPrintlnBuildVersion();

    OTV0P2BASE::LED_HEATCALL_ON();

    // Give plenty of time for the XTal to settle.
    delay(1000);

    // Have 32678Hz clock at least running before going any further.
    // Check that the slow clock is running reasonably OK, and tune the fast one to it.
    if(!::OTV0P2BASE::HWTEST::calibrateInternalOscWithExtOsc()) { panic(F("Xtal")); } // Async clock not running or can't tune.
//    if(!::OTV0P2BASE::HWTEST::check32768HzOsc()) { panic(F("xtal")); } // Async clock not running correctly.

    // Buttons should not be activated DURING boot for user-facing boards; an activated button implies a fault.
    // Check buttons not stuck in the activated position.
    if(fastDigitalRead(BUTTON_MODE_L) == LOW) { panic(F("b")); }

    // Collect full set of environmental values before entering loop() in normal mode.
    // This should also help ensure that sensors are properly initialised.

    // No external sensors are *assumed* present if running alt main loop
    // This may mean that the alt loop/POST will have to initialise them explicitly,
    // and the initial seed entropy may be marginally reduced also.
    const int cV = Supply_cV.read();
    DEBUG_SERIAL_PRINT_FLASHSTRING("V: ");
    DEBUG_SERIAL_PRINT(cV);
    DEBUG_SERIAL_PRINTLN();
    const int heat = TemperatureC16.read();
    DEBUG_SERIAL_PRINT_FLASHSTRING("T: ");
    DEBUG_SERIAL_PRINT(heat);
    DEBUG_SERIAL_PRINTLN();
// SHT21 not present by default.
//    const uint8_t rh = RelHumidity.read();
//    DEBUG_SERIAL_PRINT_FLASHSTRING("RH%: ");
//    DEBUG_SERIAL_PRINT(rh);
//    DEBUG_SERIAL_PRINTLN();
    const int light = AmbLight.read();
    DEBUG_SERIAL_PRINT_FLASHSTRING("L: ");
    DEBUG_SERIAL_PRINT(light);
    DEBUG_SERIAL_PRINTLN();
    const int tempPot = TempPot.read();
    DEBUG_SERIAL_PRINT_FLASHSTRING("temp pot: ");
    DEBUG_SERIAL_PRINT(tempPot);
    DEBUG_SERIAL_PRINTLN();

//    ValveDirect.read();

    // Initialised: turn main/heatcall UI LED off.
    OTV0P2BASE::LED_HEATCALL_OFF();

    // Do OpenTRV-specific (late) setup.
}


//========================================
// MAIN LOOP
//========================================
/**
 * @brief   Sleep in low power mode if not sleeping.
 */
void loop()
{
    // Ensure that serial I/O is off while sleeping.
    OTV0P2BASE::powerDownSerial();
    // Power down most stuff (except radio for hub RX).
    OTV0P2BASE::minimisePowerWithoutSleep();

    delay(100);
    const int cV = Supply_cV.read();
    DEBUG_SERIAL_PRINT_FLASHSTRING("V: ");
    DEBUG_SERIAL_PRINT(cV);
    DEBUG_SERIAL_PRINTLN();
    ValveDirect.poll();
    const uint16_t mi = OTV0P2BASE::analogueNoiseReducedRead(MOTOR_DRIVE_MI_AIN, DEFAULT);
    DEBUG_SERIAL_PRINT_FLASHSTRING("I: ");
    DEBUG_SERIAL_PRINT(mi);
    DEBUG_SERIAL_PRINTLN();

}



/**
 * @note    Power consumption figures (all in mA).
 * Date/commit:         Device: Wake (Sleep) @ Voltage
 * 20161111/0e6ec96     REV7:   1.5 (1.1) @ 2.5 V       REV11:  0.45 (0.03) @ 2.5 V
 * 20161111/f2eed5e     REV7:   0.46 (0.04) @ 2.5 V
 */
 
