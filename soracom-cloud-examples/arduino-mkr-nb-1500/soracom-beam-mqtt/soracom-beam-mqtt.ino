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

#include <ArduinoMqttClient.h>
#include <MKRNB.h>

// Debug options
#define PRINT_AT        true   // Show or hide AT command output from the modem


// Soracom network configuration. Default PIN is 0000
const char PINNUMBER[]     = "0000";
char apn[] = "soracom.io";
char user[] = "sora";
char pass[] = "sora";

// Soracom Beam MQTT Broker and Port
const char broker[] = "beam.soracom.io"; //
int        port     = 1883;

// Pub/Sub Topics 
const char subscribeTopic[]  = "SOME_TOPIC/to"; 
const char publishTopic[]  = "SOME_TOPIC/from";

NBClient client;
GPRS gprs;
NB nbAccess(PRINT_AT);
MqttClient mqttClient(client);

int LED = 6; // Onboard LED

// Publish interval
long previousMillis = 0; 
long interval = 10000;

void setup() {
  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, LOW);

  // Send Poweron pulse
  pinMode(SARA_PWR_ON, OUTPUT);
  digitalWrite(SARA_PWR_ON, HIGH);
  delay(150);
  digitalWrite(SARA_PWR_ON, LOW);
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
 Serial.print(">>> Resetting modem to default settings...");
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
 Serial.println(" done!");
  

  Serial.print("Attempting to connect network");
   // connection state
  bool connected = false;

  while (!connected) {
    if ((nbAccess.begin(PINNUMBER, apn, user, pass) == NB_READY) &&
        (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(subscribeTopic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(subscribeTopic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(subscribeTopic);
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis(); 
  
  mqttClient.poll();
  delay(2000);

  // Enforce Interval
  if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;

        // Call our publishing function
        publishSensorVal();
      
  }
}

void publishSensorVal() {
    // Read a sensor value from A0
    int sensorVal = analogRead(0);   

    // Publish sensor value to Publish Topic
    mqttClient.beginMessage(publishTopic);
    mqttClient.print(String(sensorVal));
    mqttClient.endMessage();
}

void handleCommand(String cmd) {
  if (cmd == "on"){
    digitalWrite(LED, HIGH);
  }else if(cmd == "off"){
    digitalWrite(LED, LOW);
  }
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  String content = "";
  char character;
  
  while (mqttClient.available()) {
    character = (char)mqttClient.read();
    content.concat(character);
  }

  handleCommand(content); 

  Serial.println(content);
  Serial.println(); Serial.println(); // double spacing
}