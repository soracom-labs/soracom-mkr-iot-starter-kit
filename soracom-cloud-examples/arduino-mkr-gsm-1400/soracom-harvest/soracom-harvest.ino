#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <MKRGSM.h>

// Debug options
#define PRINT_AT        true   // Show or hide AT command output from the modem

// PIN Number
const char PINNUMBER[]     = "";
char apn[] = "soracom.io";
char user[] = "sora";
char pass[] = "sora";

// Server details
const char server[] = "unified.soracom.io";
const int  port = 80; 


GSMModem modem;
GSM gsmAccess(PRINT_AT); // set true to see and debug modem AT commands on the Serial port
GPRS gprs;
GSMClient client;
HttpClient http(client, server, port);

// connection state
bool connected = false;

// Publish interval
long previousMillis = 0; 
long interval = 20000; // milliseconds

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(apn, user, pass) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      connected = false;
      delay(1000);
    }
  }

  Serial.println("You're connected to the network");  
}

void loop() {
  unsigned long currentMillis = millis(); 

  if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;

      // Construct the JSON data to send
      StaticJsonDocument<200> doc;
      doc["time"] = millis();
      doc["sensor_a0"] = analogRead(0);
      char jsonBuffer[512];
      serializeJson(doc, jsonBuffer); // print to client
      post_data(jsonBuffer); 
  }
}


void post_data(String postData) {
  Serial.println("making POST request");
  String contentType = "application/json";

  http.post("/", contentType, postData);

  // read the status code and body of the response
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

}
