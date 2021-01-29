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

#include <Arduino.h>            // Base Arduino Library
#include <MKRNB.h>              // Arduino MKRNB library
#include <BlynkSimpleStream.h>

char apn[] = "soracom.io";
char userpass[] = "sora";


// Add your Blynk auth key here!
const char auth[] = "";

NB nbAccess(true);
GPRS gprs;
NBClient client;

// Base variables
const unsigned long baud = 115200;

// Helper function to connect to a data network
void SoraGprsConnect() {
  while (true) {
    if (nbAccess.begin("", apn, userpass, userpass) == NB_READY && gprs.attachGPRS() == GPRS_READY) {
      if (Serial) {
        Serial.println(" connected!");
        client.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
        
        Blynk.begin(client, auth);
      }
      return;
    } else {
      if (Serial) Serial.print(".");
      delay(1000);
    }
  }
}


// Device setup
void setup() {

  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, LOW);

  // Send Poweron pulse
  pinMode(SARA_PWR_ON, OUTPUT);
  digitalWrite(SARA_PWR_ON, HIGH);
  delay(150);
  digitalWrite(SARA_PWR_ON, LOW);
  // Start the USB serial interface
  Serial.begin(baud);

  // Check to see if the USB serial interface is connected, otherwise continue without it after 1 minute
  for (int i = 0; i < 60; i++) {
    if (Serial) break;
    delay(1000);
  }
  if (Serial) Serial.println(">>> Starting Soracom MKR NB 1500 demo with Grove Sensor Kit!");

  
  // Turn on the modem
  if (Serial) Serial.print(">>> Turning on modem...");
  if (MODEM.begin()) {
    if (Serial) Serial.println(" done!");
  } else {
    if (Serial) Serial.println(" error, could not turn on modem! Try power-cycling.");
    return;
  }


  while (!MODEM.noop());
  MODEM.sendf("ATI9");
  MODEM.waitForResponse(2000);
  
  /**
   * On some u-blox R410M modems with older firmware, the frequency bands are incorrectly locked to
   * specific regions even though the MNO profile is set to 0 == default (global coverage). These
   * AT commands will reset the modem to the correct factory default settings. Generally, you only
   * need to run this one time. Once your MKR NB 1500 has connected to a network, you can remove
   * this code section since the correct connection settings will be stored directly on the modem.
   */
  // Run AT commands to reset the modem to global default settings
  if (Serial) Serial.print(">>> Resetting modem to default settings...");
  MODEM.sendf("AT+CFUN=0");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+UMNOPROF=2");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+CFUN=15");
  MODEM.waitForResponse(6000);
  MODEM.sendf("AT+CFUN=0");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+UMNOPROF=0");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+CFUN=15");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+URAT=7");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+URAT?");
  MODEM.waitForResponse(2000);
  MODEM.sendf("AT+UBANDMASK?");
  MODEM.waitForResponse(2000);
  if (Serial) Serial.println(" done!");

  if (Serial) Serial.print(">>> Connecting to Cat-M1 network...");
  SoraGprsConnect(); // Attach to the data network
}

// Running looping function
void loop() {
  if (Serial) Serial.println("-----------------------");

  // Make sure the device is still connected to CatM network
  if (nbAccess.isAccessAlive()) {
    if (Serial) Serial.println(">>> Modem is still connected to Cat-M1 network!");
  } else {
    if (Serial) Serial.print(">>> Disconnected from Cat-M1 network. Reconnecting...");
    nbAccess.shutdown(); // Reset the modem
    SoraGprsConnect(); // Attach to the data network
  }

  Blynk.run();
}