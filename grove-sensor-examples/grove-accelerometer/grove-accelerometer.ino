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

#include <Wire.h>
#include <ADXL345.h>

#define SERIAL_SPEED 9600

ADXL345 sensor;

bool timerExpired(unsigned long timeDelta = 1000)
{
  static unsigned long tsStart = millis(); // this value will persist, see note above.

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
  Serial.println(F("ADXL345 starting.."));

  sensor.powerOn(); // using the default TWI (I2C) pins on the MKR
                    // SDA on pin 11 &
                    // SCL on pin 12
}

void loop()
{
  static unsigned int loopCounter = 0;  // this value will persist, see notes.

  if (timerExpired())
  {
    int x, y, z;

    loopCounter = loopCounter + 1;

    sensor.readXYZ(&x, &y, &z);

    Serial.print(F("Force X:"));
    Serial.print(x, DEC);

    Serial.print(F(" Y:"));
    Serial.print(y, DEC);

    Serial.print(F(" Z:"));
    Serial.println(z, DEC);
  };

  if (loopCounter == 5)
  {
    double acceleration[3];

    loopCounter = 0;

    sensor.getAcceleration(acceleration);

    Serial.print(F("Acceleration X:"));
    Serial.print

    (acceleration[0], DEC);

    Serial.print(F(" Y:"));
    Serial.print(acceleration[1], DEC);

    Serial.print(F(" Z:"));
    Serial.println(acceleration[2], DEC);
  };
}
