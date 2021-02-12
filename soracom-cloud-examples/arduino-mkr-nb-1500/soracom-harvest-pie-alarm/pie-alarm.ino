#include <ArduinoHttpClient.h>
#include <Bounce2.h>
#include <MKRNB.h>

// Debug options
#define PRINT_AT        true   // Show or hide AT command output from the modem
#define REED_INPUT 2
#define BZR_PIN 3

Bounce reedSwitch = Bounce();

// PIN Number
const char PINNUMBER[]     = "";
char apn[] = "soracom.io";
char user[] = "sora";
char pass[] = "sora";

// Server details
const char server[] = "unified.soracom.io";
const int  port = 80; 


NBClient client;
GPRS gprs;
NB nbAccess(PRINT_AT);
HttpClient http(client, server, port);

// connection state
bool connected = false;

// Publish interval
long previousMillis = 0; 
long interval = 20000; // milliseconds


void setup() { 
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  reedSwitch.attach(REED_INPUT, INPUT);
  reedSwitch.interval(100);

  pinMode(BZR_PIN, OUTPUT);
  digitalWrite(BZR_PIN, LOW);

  start_and_connect_modem();
}

void loop() {
  reedSwitch.update();

  if (reedSwitch.changed()) {

    int magnet_state = reedSwitch.read();
    magnet_state = reedSwitch.read();
    Serial.println(magnet_state ? F("MAGNET") : F("NO MAGNET"));
    if(magnet_state == 0){
      digitalWrite(BZR_PIN, HIGH);
      delay(500);
      digitalWrite(BZR_PIN, LOW);
    }
    
    // Make sure the device is still connected to CatM network
    if (nbAccess.isAccessAlive()) {
      // Enforce Interval
        // Construct the JSON data to send
        String data = "{\"magnet\":\""; 
        data += magnet_state;
        data += "\"}";
        
        post_data(data); 
      }
    } else {
      Serial.println("Modem disconnected, reconnecting");
      connected = false;
      nbAccess.shutdown();
      connect_modem();
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

void start_and_connect_modem(){
  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, LOW);
  pinMode(SARA_PWR_ON, OUTPUT);
  
  // Send Poweron pulse
  digitalWrite(SARA_PWR_ON, HIGH);
  delay(150);
  digitalWrite(SARA_PWR_ON, LOW);
  delay(500);
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
  
  // Run AT commands to reset the modem to global default settings
  Serial.print(">>> Resetting modem to default settings...");
  MODEM.sendf("AT+CFUN=0");
  MODEM.waitForResponse(6000);
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
  
  delay(2000);
  // attempt to connect to GSM and GPRS:
  Serial.print("Attempting to connect to GSM and GPRS");
  Serial.println();
  connect_modem();
  
}

void connect_modem(){
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
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
}

