/*  Copyright (c) 2020 SORACOM
    MIT Licence

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <Arduino.h>
#include <Arduino_PMIC.h>

#define SERIAL_SPEED 9600

bool timerExpired(unsigned long timeDelta = 1000)
{
  static unsigned long tsStart = millis(); // this value will persist,, see notes in README

  unsigned long tsNow = millis();
  unsigned long tsEnd = tsStart + timeDelta;

  if(tsNow <= tsEnd)
  { // not expired
    return false;
  }
  else
  { // expired
  tsStart = tsNow;
  tsEnd = tsStart + timeDelta;
  return true;
  }
};

void setup()
{
  Serial.begin(SERIAL_SPEED);

  while (!Serial)
      delay(1000); // remove this while loop to start the sketch at power-on, else the sketch will wait for a serial connection

  Serial.println();
  Serial.print(F("Starting PMIC test.. "));
  Serial.println(PMIC.begin(), DEC);

  Serial.print(F("enableCharge() : "));
  Serial.println(PMIC.enableCharge(), DEC);

  Serial.print(F("getChargeCurrent() : "));
  Serial.println(PMIC.getChargeCurrent(), DEC);

  Serial.print(F("getChargeFault() : "));
  Serial.println(PMIC.getChargeFault(), DEC);

  Serial.print(F("getChargeVoltage() : "));
  Serial.println(PMIC.getChargeVoltage(), DEC);

  Serial.print(F("getInputCurrentLimit() : "));
  Serial.println(PMIC.getInputCurrentLimit(), DEC);

  Serial.print(F("getInputVoltageLimit() : "));
  Serial.println(PMIC.getInputVoltageLimit(), DEC);

  Serial.print(F("getMinimumSystemVoltage() : "));
  Serial.println(PMIC.getMinimumSystemVoltage(), DEC);

  Serial.print(F("getPreChargeCurrent() : "));
  Serial.println(PMIC.getPreChargeCurrent(), DEC);

  Serial.print(F("getTermChargeCurrent() : "));
  Serial.println(PMIC.getTermChargeCurrent(), DEC);

  Serial.print(F("isBattConnected() : "));
  Serial.println(PMIC.isBattConnected(), DEC);

  Serial.print(F("isBatteryInOverVoltage() : "));
  Serial.println(PMIC.isBatteryInOverVoltage(), DEC);

  Serial.print(F("isHot() : "));
  Serial.println(PMIC.isHot(), DEC);

  Serial.print(F("isPowerGood() : "));
  Serial.println(PMIC.isPowerGood(), DEC);

  Serial.print(F("isWatchdogExpired() : "));
  Serial.println(PMIC.isWatchdogExpired(), DEC);

  Serial.print(F("canRunOnBattery() : "));
  Serial.println(PMIC.canRunOnBattery(), DEC);

  Serial.print(F("chargeStatus() : 0x"));
  Serial.print(PMIC.chargeStatus(), HEX);

  switch (PMIC.chargeStatus())
  {
  case NOT_CHARGING : Serial.println(F(" NOT_CHARGING")); break;
  case PRE_CHARGING : Serial.println(F(" PRE_CHARGING")); break;
  case FAST_CHARGING : Serial.println(F(" FAST_CHARGING")); break;
  case CHARGE_TERMINATION_DONE : Serial.println(F(" CHARGE_TERMINATION_DONE")); break;

  default: Serial.println(F(" Error!")); break;
  }
  //PMIC.end();
}

void loop()
{
  if (timerExpired(10e3)) // 10e3 = 10,000 = 10 seconds
  {
    Serial.print(F("PMIC.chargeStatus() : 0x"));
    Serial.print(PMIC.chargeStatus(), HEX);

    switch (PMIC.chargeStatus())
    {
      case NOT_CHARGING : Serial.println(F(" NOT_CHARGING")); break;
      case PRE_CHARGING : Serial.println(F(" PRE_CHARGING")); break;
      case FAST_CHARGING : Serial.println(F(" FAST_CHARGING")); break;
      case CHARGE_TERMINATION_DONE : Serial.println(F(" CHARGE_TERMINATION_DONE")); break;

      default: Serial.println(F(" Error!")); break;
    }

    Serial.print(F("ADC_BATTERY : "));
    Serial.println(analogRead(ADC_BATTERY) * (4.3 / 1023.0), DEC); // See note above for info on the conversion
  }
}