copyright 2021 (c) Soracom

MIT Licence

# Power Manager (BQ24195)
This sketch will read data from the onboard Power Manager.  Output from the sensor will be dumped to the Serial port on the IDE, changes in PSU and battery level will change the output.

![photo_power_manager](../../content/power_manager.png)

Additional information on the onboard Power Manager is available online at [Power Manager (BQ24195)](https://www.arduino.cc/reference/en/libraries/arduino_bq24195/).

## Required Arduino Libraries:

- [Arduino](https://github.com/arduino/Arduino)
- [PMIC](https://github.com/arduino-libraries/Arduino_BQ24195)

## Program setup

### Getting started

1. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
1. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify and upload the sketch.
1. Start the serial monitor, the serial connection speed is set in the running sketch by `#define SERIAL_SPEED 9600` at 9600 bps, serial speed in the IDE should be set to the same.
1. The Power Manager (BQ24195) object will emit readings after the connection from the IDE to Serial port has been established.

Please note;
- If you have problems compiling the code add the following `#define` statement close to the top of `BQ24195.cpp`  This is a known issue and is detailed at the link following. [Arduino_BQ24195](https://github.com/arduino-libraries/Arduino_BQ24195/issues/6).

````cpp
      #define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
````
- Voltage conversion in the example code is based on the 'MKRZERO_Read_Battery_Voltage by Arduino_Genuino' project on [Arduino Create](https://create.arduino.cc).
- The Power Manager module code for Arduino is BETA, your results may vary.  Check on the Arduino site for updates to this software.
- The timerExpired function has limited accuracy and is expected to drift over time.

### Digital Ports

The Power Manager is connected using one of the I2C buses on the Arduino.  No additional connectivity is required.

## Console output at the Serial port
The sketch will wait for the IDE to attach to the Serial port

```text
17:10:19.110 -> Starting PMIC test.. 1
17:10:19.110 -> enableCharge() : 1
17:10:19.110 -> getChargeCurrent() : 0.5120000243
17:10:19.110 -> getChargeFault() : 0
17:10:19.110 -> getChargeVoltage() : 4.1119999886
17:10:19.110 -> getInputCurrentLimit() : 3.0000000000
17:10:19.110 -> getInputVoltageLimit() : 3.8800001144
17:10:19.110 -> getMinimumSystemVoltage() : 3.5000000000
17:10:19.110 -> getPreChargeCurrent() : 0.2560000122
17:10:19.110 -> getTermChargeCurrent() : 0.2560000122
17:10:19.110 -> isBattConnected() : 0
17:10:19.110 -> isBatteryInOverVoltage() : 0
17:10:19.110 -> isHot() : 0
17:10:19.110 -> isPowerGood() : 1
17:10:19.110 -> isWatchdogExpired() : 0
17:10:19.110 -> canRunOnBattery() : 0
17:10:19.110 -> chargeStatus() : 0x10 PRE_CHARGING
17:10:29.130 -> PMIC.chargeStatus() : 0x20 FAST_CHARGING
17:10:29.130 -> ADC_BATTERY : 3.4761485826
17:10:39.117 -> PMIC.chargeStatus() : 0x20 FAST_CHARGING
17:10:39.117 -> ADC_BATTERY : 3.3752688172
17:10:49.134 -> PMIC.chargeStatus() : 0x10 PRE_CHARGING
17:10:49.134 -> ADC_BATTERY : 3.2743890518
```

## Code Cuts
- The power manager object is created when you include the code module, `#include <Arduino_PMIC.h>`
- Start the power manager interface, `PMIC.begin();`
- Read data from the power manager chip and dump it to the Serial interface on the IDE, `Serial.println(PMIC.getChargeVoltage(), DEC);`

### Static Variable
The content (value) of a 'static' variable persists, for example value of 'tsStart' in function timerExpired() persists between calls to the function, the value of the variable is initialized the first time the function is called and retains its setting between calls.

```c
	static unsigned long tsStart = millis();
```

### Using FLASH memory for static data
The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.

```c
	Serial.print(F("Starting PMIC test.. "));
```
