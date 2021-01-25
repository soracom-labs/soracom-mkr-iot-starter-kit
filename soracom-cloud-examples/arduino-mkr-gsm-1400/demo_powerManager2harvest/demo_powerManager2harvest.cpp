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

#include <MKRGSM.h>

#define SERIAL_SPEED 9600

#define PINNUMBER ""
#define GPRS_APN       "soracom.io"
#define GPRS_LOGIN     "sora"
#define GPRS_PASSWORD  "sora"

#define SERVER "unified.soracom.io"
#define PORT 80

bool timerExpired(unsigned long timeDelta = 1000)
{
  static unsigned long tsStart = millis(); // this value will persist,, see notes in README

  unsigned long tsNow = millis();
  unsigned long tsEnd = tsStart + timeDelta;

  if (tsNow <= tsEnd)
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

GSMModem modem;
GSMClient client;
GPRS gprs;
GSM gsmAccess(false);

boolean notConnected = true;

void setup()
{
  Serial.begin(SERIAL_SPEED);
  SerialGSM.begin(SERIAL_SPEED);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(250);

  for (int loopCount = 0; loopCount <= 60; loopCount++)
    while (!Serial) // Wait for a serial connection, timeout after 60 loops (close to 1 min)
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(1000);
    }

  while (!SerialGSM);

  Serial.println();
  Serial.print("Starting PMIC test.. ");
  Serial.println(PMIC.begin(), DEC);

  Serial.print("enableCharge() : ");
  Serial.println(PMIC.enableCharge(), DEC);

  Serial.print("getChargeCurrent() : ");
  Serial.println(PMIC.getChargeCurrent(), DEC);

  Serial.print("getChargeFault() : ");
  Serial.println(PMIC.getChargeFault(), DEC);

  Serial.print("getChargeVoltage() : ");
  Serial.println(PMIC.getChargeVoltage(), DEC);

  Serial.print("getInputCurrentLimit() : ");
  Serial.println(PMIC.getInputCurrentLimit(), DEC);

  Serial.print("getInputVoltageLimit() : ");
  Serial.println(PMIC.getInputVoltageLimit(), DEC);

  Serial.print("getMinimumSystemVoltage() : ");
  Serial.println(PMIC.getMinimumSystemVoltage(), DEC);

  Serial.print("getPreChargeCurrent() : ");
  Serial.println(PMIC.getPreChargeCurrent(), DEC);

  Serial.print("getTermChargeCurrent() : ");
  Serial.println(PMIC.getTermChargeCurrent(), DEC);

  Serial.print("isBattConnected() : ");
  Serial.println(PMIC.isBattConnected(), DEC);

  Serial.print("isBatteryInOverVoltage() : ");
  Serial.println(PMIC.isBatteryInOverVoltage(), DEC);

  Serial.print("isHot() : ");
  Serial.println(PMIC.isHot(), DEC);

  Serial.print("isPowerGood() : ");
  Serial.println(PMIC.isPowerGood(), DEC);

  Serial.print("isWatchdogExpired() : ");
  Serial.println(PMIC.isWatchdogExpired(), DEC);

  Serial.print("canRunOnBattery() : ");
  Serial.println(PMIC.canRunOnBattery(), DEC);

  Serial.print("chargeStatus() : 0x");
  Serial.print(PMIC.chargeStatus(), HEX);

  switch (PMIC.chargeStatus())
  {
    case NOT_CHARGING : Serial.println(" NOT_CHARGING"); break;
    case PRE_CHARGING : Serial.println(" PRE_CHARGING"); break;
    case FAST_CHARGING : Serial.println(" FAST_CHARGING"); break;
    case CHARGE_TERMINATION_DONE : Serial.println(" CHARGE_TERMINATION_DONE"); break;

    default: Serial.println(" Error!"); break;
  }

  PMIC.end();

  Serial.print("Starting Modem, GSM and GPRS ");

  while (notConnected) {
    digitalWrite(LED_BUILTIN, LOW);
    if (modem.begin() == true) {

      Serial.print(modem.getIMEI());
      Serial.print(" Modem started! ");

      if (gsmAccess.begin(PINNUMBER) == GSM_READY) {

        Serial.print(" GSM_READY ");

        if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY) {

          Serial.println(" GPRS_READY ");
          notConnected = false;
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
    Serial.print(".");
    delay(1000);
  }

  if (client.connect(SERVER, PORT)) {

    char message[] = "{\"STATUS\":\"RESTARTING\"}";

    Serial.print("TCP Connection to server is UP!, sending '");
    Serial.print(message);
    Serial.println("'");

    client.println("POST / HTTP/1.1");
    client.print("Host: ");
    client.println(SERVER);
    client.println("User-Agent: Soracom/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(strlen(message));

    client.println();
    client.println(message);
  }

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
}
void loop()
{
  if (timerExpired(600e3)) // 0Hr 10Min
  {
    PMIC.begin();

    Serial.print("PMIC.chargeStatus() : 0x");
    Serial.print(PMIC.chargeStatus(), HEX);

    switch (PMIC.chargeStatus())
    {
      case NOT_CHARGING : Serial.println(" NOT_CHARGING"); break;
      case PRE_CHARGING : Serial.println(" PRE_CHARGING"); break;
      case FAST_CHARGING : Serial.println(" FAST_CHARGING"); break;
      case CHARGE_TERMINATION_DONE : Serial.println(" CHARGE_TERMINATION_DONE"); break;

      default: Serial.println(" Error!"); break;
    }

    Serial.print("ADC_BATTERY : ");
    Serial.println(analogRead(ADC_BATTERY) * (4.3 / 1023.0), DEC); // See note A for info on the conversion

    notConnected = !gsmAccess.isAccessAlive();

    while (notConnected) {
      Serial.print("Restarting the GSM Stack!..");
      digitalWrite(LED_BUILTIN, LOW);

      gsmAccess.shutdown();

      if (gsmAccess.begin(PINNUMBER) == GSM_READY) {

        Serial.print(" GSM_READY ");

        if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY) {

          Serial.println(" GPRS_READY ");
          notConnected = false;
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
      Serial.print(".");
      delay(1000);
    }
    PMIC.end();

    if (gsmAccess.isAccessAlive() && client.connect(SERVER, PORT)) {

      Serial.println("Sending update to server..");

      String message = String("{\"ADC_BATTERY\":");

      message = message + String((analogRead(ADC_BATTERY) * (4.3 / 1023.0)), DEC);
      message = message + String("}");

      Serial.print("TCP Connection to server is UP!, sending '");
      Serial.print(message);
      Serial.println("'");

      client.println("POST / HTTP/1.1");
      client.print("Host: ");
      client.println(SERVER);
      client.println("User-Agent: Soracom/1.0");
      client.println("Connection: close");
      client.print("Content-Length: ");
      client.println(message.length());

      client.println();
      client.println(message.c_str());
    }
    else {
      Serial.println("Skipping update to server..");
    }
  }
}