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
#include <MKRGSM.h>

#define SERIAL_SPEED 9600

#define PINNUMBER     ""
#define GPRS_APN      "soracom.io"
#define GPRS_LOGIN    "sora"
#define GPRS_PASSWORD "sora"

#define HARVEST_SERVER "unified.soracom.io"
#define HARVEST_PORT 80

GSMModem modem;
GSM gsmAccess(false); // set true to see and debug modem AT commands on the Serial port
GPRS gprs;
GSMClient client;

boolean notConnected = true;

void setup()
{
  Serial.begin(SERIAL_SPEED);
  SerialGSM.begin(115200);

  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(250);

  for (int loopCount = 0; loopCount <= 60; loopCount++)
    while (!Serial) // Wait for a serial connection, timeout after 60 loops (close to 1 min)
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(1000);
    }

  Serial.println();
  Serial.println(F("Starting Soracom HARVEST test.. "));

  while (!SerialGSM)
    delay(1000);

  Serial.println(F("Starting Modem, GSM, GPRS and Soracom Cloud connection"));

  while (notConnected)
  {
    digitalWrite(LED_BUILTIN, LOW);

    if (gsmAccess.begin(PINNUMBER, true, true) == GSM_READY)
    {
      delay(1000);
      Serial.print(F("\t[1/4] GSM_READY "));
      Serial.print(F(" Modem IMEI : "));
      Serial.print(modem.getIMEI());
      Serial.print(F(" SIM ICCID : "));
      Serial.println(modem.getICCID());

      if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD, true) == GPRS_READY)
      {
        delay(1000);
        Serial.println(F("\t[2/4] GPRS_READY "));

        if (client.connect(HARVEST_SERVER, HARVEST_PORT) == true)
        {
          delay(1000);
          Serial.println(F("\t[3/4] Soracom Cloud connection ready "));
          notConnected = false;
        }
        else
        {
          client.stop();
        }
      }
      else
      {
        gprs.detachGPRS();
      }
    }
    else
    {
      gsmAccess.shutdown();
    }

    Serial.print(F("."));
    delay(1000);
  }

  if (client.connect(HARVEST_SERVER, HARVEST_PORT))
  {
    Serial.println(F("\t[4/4] Sending message to Soracom Harvest"));

    digitalWrite(LED_BUILTIN, HIGH);

    char message[] = "{\"A\":10,\"B\":11,\"C\":12}";

    Serial.print(F("Soracom Cloud connection is UP, sending '"));
    Serial.print(message);
    Serial.print(F("', timeout is : "));
    Serial.println(client.getTimeout(), DEC);

    client.println(F("POST / HTTP/1.1"));
    client.print(F("Host: "));
    client.println(HARVEST_SERVER);
    client.println(F("User-Agent: Soracom/1.0"));
    client.println(F("Connection: close"));
    client.println(F("Content-type: application/json"));
    client.println(F("X-Clacks-Overhead: GNU Terry Pratchett"));

    client.print(F("Content-Length: "));
    client.println(strlen(message));

    client.println();
    client.println(message);

    if (client.ready() == 0)
    {
      Serial.print(F("."));
      delay(500);
    }

    digitalWrite(LED_BUILTIN, LOW);
  }

  while (client.connected() || client.available())
  {
    if (client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
  }

  client.stop();
  gprs.detachGPRS();
  gsmAccess.shutdown();
}
void loop()
{
  delay(1000);
}