
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

#define REED_INPUT 2
#define SERIAL_SPEED 9600

#include <Arduino.h>
#include <Bounce2.h>

Bounce reedSwitch = Bounce();

uint32_t reedCounter = 0;

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
  Serial.println(F("Starting reed switch test.."));

  reedSwitch.attach(REED_INPUT, INPUT);
  reedSwitch.interval(25);
}

void loop()
{
  reedSwitch.update();

  if (reedSwitch.changed())
  {
    reedCounter = reedCounter + 1;

    Serial.print(F("BTN Update ["));
    Serial.print(reedCounter, DEC);
    Serial.print(F("] : "));

    Serial.println(reedSwitch.read() ? F("MAGNET") : F("NO MAGNET"));

    Serial.print(F("BTN State : "));
    Serial.println(reedSwitch.read(), DEC);
  }

  if(timerExpired())
    Serial.print(F("."));
}