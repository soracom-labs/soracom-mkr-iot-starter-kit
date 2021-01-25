/*  Copyright (c) 2021 SORACOM
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

void setup() {

  Serial.begin(SERIAL_SPEED);                 // connection speed for the serial port, this should match the setting in the IDE.

  while (!Serial)                     // wait for the serial port to connect, this will wait forever
    delay(1000);

  Serial.println();
  Serial.println(F("Hello World!"));  // print the text 'Hello World!' to the serial output port

  pinMode(LED_BUILTIN, OUTPUT);       // set the hardware pin with the LED attached to OUTPUT, this sets the pin up as a driver
  digitalWrite(LED_BUILTIN, LOW);     // set the hardware pin to LOW, the LED is OFF
}

void loop() {
  static unsigned long timeStamp = 0;

  if(millis() - timeStamp > 1000)
  {
    timeStamp = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
                                      // change the state of the LED from OFF to ON / ON to OFF
    Serial.print(F("."));
                                      // at each one second interval the LED will switch and a '.' is  printed to the serial port
  }
}