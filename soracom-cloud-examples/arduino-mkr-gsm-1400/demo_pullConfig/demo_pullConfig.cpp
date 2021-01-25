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
#include <ArduinoJson.h>

const unsigned char http_GET[] = "\
GET /v1/userdata HTTP/1.1\r\n\
Host: metadata.soracom.io\r\n\
User-Agent: Soracom/1.0\r\n\
Accept: application/json\r\n\
X-Clacks-Overhead: GNU Terry Pratchett\r\n\
Connection: close\r\n";

const char http_POST[] = "\
POST / HTTP/1.1\r\n\
Host: %s\r\n\
User-Agent: Soracom/1.0\r\n\
Accept: application/json\r\n\
X-Clacks-Overhead: GNU Terry Pratchett\r\n\
Connection: close\r\n\
Content-Length: %i\r\n\
\r\n\
{\\\"%s\\\":\\\"%s\\\", \\\"%s\\\":\\\"%s\\\"}";

const char http_POST_payload[] = "\
{\\\"%s\\\":\\\"%s\\\", \\\"%s\\\":\\\"%s\\\"}";

#define SERIAL_SPEED 9600

#define PINNUMBER     ""
#define GPRS_APN      "soracom.io"
#define GPRS_LOGIN    "sora"
#define GPRS_PASSWORD "sora"

#define METADATA_SERVER "metadata.soracom.io"
#define METADATA_PORT 80

GSMModem modem;
GSM gsmAccess(false); // set true to debug modem AT commands
GPRS gprs;
GSMClient httpGET(true); // calls are Sync

boolean notConnected = true;

void hwReset() // run a hardware reset for the modem hardware if it fails to start

{
  pinMode(GSM_RESETN, OUTPUT);

  digitalWrite(GSM_RESETN, HIGH);
  delay(2000);

  digitalWrite(GSM_RESETN, LOW);
  delay(5000);
}

String rmQuote(String jsonString) // remove any quotes from within the string
{
  String tmpString;

  tmpString.reserve(jsonString.length());

  for (unsigned int i = 0; i < jsonString.length(); i++)
  {
    if (jsonString.charAt(i) == '"')
      i++; // skip copying one char

    tmpString = tmpString + jsonString.charAt(i);
  }

  return tmpString;
};

String addQuote(String jsonString) // remove any quotes from within the string
{
  String tmpString;

  tmpString.reserve(jsonString.length());

  for (unsigned int i = 0; i < jsonString.length(); i++)
  {
    if (jsonString.charAt(i) == '"')
      tmpString = tmpString + "\\";

    tmpString = tmpString + jsonString.charAt(i);
  }

  return tmpString;
};

void setup()
{
  Serial.begin(SERIAL_SPEED);
  SerialGSM.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(250);

  for (int i = 0; i <= 240; i++)
    while (!Serial) // wait for the Serial port to become active, timeout after 60 seconds
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // show a fast-busy while we wait for a serial connection
      delay(250);
    }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println();
  Serial.println(F("Starting Soracom CONFIG test.. "));

  while (!SerialGSM)
    delay(1000);

  Serial.println(F("Starting Modem, GSM, GPRS and Soracom Cloud connection "));

  gsmAccess.setTimeout(25e3);
  gprs.setTimeout(25e3);
  httpGET.setTimeout(25e3);

  while (notConnected)
  {
    boolean gprs_status = false;

    digitalWrite(LED_BUILTIN, LOW);

    while ((gsmAccess.isAccessAlive() != 1))
    {
      gprs_status = false;

      Serial.print(F("\t[1/6] "));
      switch (gsmAccess.begin(PINNUMBER, true, true))
      {
      case GSM3_NetworkStatus_t::ERROR:
        Serial.println(F("ERROR"));
        hwReset();
        break;
      case GSM3_NetworkStatus_t::IDLE:
        Serial.println(F("IDLE"));
        break;
      case GSM3_NetworkStatus_t::CONNECTING:
        Serial.println(F("CONNECTING"));
        break;
      case GSM3_NetworkStatus_t::GPRS_READY:
        Serial.println(F("GPRS_READY"));
        break;
      case GSM3_NetworkStatus_t::TRANSPARENT_CONNECTED:
        Serial.println(F("TRANSPARENT_CONNECTED"));
        break;
      case GSM3_NetworkStatus_t::GSM_OFF:
        Serial.println(F("GSM_OFF"));
        break;
      case GSM3_NetworkStatus_t::GSM_READY:
        Serial.print(F("GSM_READY *"));
        Serial.print(F(" Modem IMEI : "));
        Serial.print(modem.getIMEI());
        Serial.print(F(" SIM ICCID : "));
        Serial.println(modem.getICCID());
        break;

      default:
        Serial.println(F(" switch into default for begin() - err?"));
        break;
      }
      delay(1000);
    }

    while (gprs_status == false)
    {
      httpGET.stop();

      Serial.print(F("\t[2/6] "));
      switch (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD, true))
      {
      case GSM3_NetworkStatus_t::ERROR:
        Serial.println(F("ERROR!"));
        gprs.detachGPRS();
        break;
      case GSM3_NetworkStatus_t::IDLE:
        Serial.println(F("IDLE"));
        break;
      case GSM3_NetworkStatus_t::CONNECTING:
        Serial.println(F("CONNECTING"));
        break;
      case GSM3_NetworkStatus_t::GSM_READY:
        Serial.println(F("GSM_READY *"));
        gprs_status = true;
        break;
      case GSM3_NetworkStatus_t::GPRS_READY:
        Serial.println(F("GPRS_READY *"));
        gprs_status = true;
        break;
      case GSM3_NetworkStatus_t::TRANSPARENT_CONNECTED:
        Serial.println(F("TRANSPARENT_CONNECTED"));
        break;

      default:
        Serial.println(F(" switch into default for attachGPRS() - err?"));
        break;
      }
      delay(1000);
    }

    while (httpGET != true)
    {
      Serial.print(F("\t[3/6] Cloud Connection to Soracom metadata, host : "));
      Serial.print(METADATA_SERVER);
      Serial.print(F(", port : "));
      Serial.print(METADATA_PORT);

      switch (httpGET.connect(METADATA_SERVER, METADATA_PORT))
      {
      case true:
        Serial.println(F("TRUE *"));
        notConnected = false;
        break;
      case false:
        Serial.println(F("FALSE"));
        break;

      default:
        break;
        Serial.println(F(" switch into default for connect() - err?"));
        break;
      }
      delay(1000);
    }

    Serial.print(F("."));
    delay(1000);

    Serial.println(F("\t[4/6] Requesting update from Soracom Metadata"));

    if (!httpGET.connected())
    {
      Serial.println("UXO : Soracom metadata has dropped the connection!..");
      notConnected = true;
      continue;
    }

    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println();

    httpGET.beginWrite(true);

    httpGET.write(http_GET);
    httpGET.println();

    httpGET.endWrite();

    digitalWrite(LED_BUILTIN, LOW);

    DynamicJsonDocument soracomMetadataConfig(100);

    while (!httpGET.available() && httpGET.connected())
    {
      Serial.print(F("."));
      delay(1000);
    }

    Serial.println(F("\t[5/6] Awaiting reply from Soracom metadata"));
    Serial.println();

    String buffer;
    buffer.reserve(200);

    while (httpGET.available())
    {
      char byte = static_cast<char>(httpGET.read());
      Serial.print(byte);
      buffer = buffer + byte;
    }

    Serial.println();
    Serial.print(F("Check status : "));
    Serial.println(buffer.indexOf(F("200 OK")) != -1 ? F("Found 200 OK") : F("UNEXPECTED HTTP STATUS"));

    Serial.print(F("Find JSON string : "));
    Serial.println(buffer.indexOf(F("{")) != -1 ? F("FOUND JSON BLOCK") : F("NOT FOUND!"));

    deserializeJson(soracomMetadataConfig, buffer.substring(buffer.indexOf("{")));

    const char *ptcl = soracomMetadataConfig[F("PTCL")];
    const char *host = soracomMetadataConfig[F("HST")];
    const char *prt = soracomMetadataConfig[F("PRT")];

    Serial.println();
    Serial.println(F("Configuration from Soracom Metadata service"));
    Serial.print(F("Application Protocol :\t"));
    Serial.println(ptcl);

    Serial.print(F("Interface Name :\t"));
    Serial.println(host);

    Serial.print(F("Interface Port :\t"));
    Serial.println(prt);

    Serial.println();

    String stringBuf;
    String modemReply_1;
    String modemReply_2;

    MODEM.send("AT+CSQ\r\n"); // Page 76 -  - https://evk-download.u-blox.com/images/downloads/Product_Docs/AT-CommandsExamples_ApplicationNote_(UBX-13001820).pdf
    Serial.print(F("waitForResponse : "));
    switch (MODEM.waitForResponse(1000, &stringBuf))
    {
    case 1:
      modemReply_1 = String(stringBuf.substring(stringBuf.indexOf(F(":")) + 2));
      modemReply_1 = addQuote(modemReply_1);

      break;
    case 2:
      modemReply_1 = F("ERROR");
      break;
    case 3:
      modemReply_1 = F("NO CARRIER");
      break;

    default:
      modemReply_1 = F("default : Err?");
      break;
    }

    Serial.println(stringBuf);

    Serial.print(F("waitForResponse : "));
    MODEM.send("AT+COPS?"); // Page 15 - https://evk-download.u-blox.com/images/downloads/Product_Docs/AT-CommandsExamples_ApplicationNote_(UBX-13001820).pdf
    switch (MODEM.waitForResponse(1000, &stringBuf))
    {
    case 1:
      modemReply_2 = String(stringBuf.substring(stringBuf.indexOf(F(":")) + 2));
      modemReply_2 = addQuote(modemReply_2);

      break;

    case 2:
      modemReply_2 = F("ERROR");
      break;

    case 3:
      modemReply_2 = F("NO CARRIER");
      break;

    default:
      modemReply_2 = F("default : Err?");
      break;
    }

    Serial.println(stringBuf);

    GSMClient httpPOST(true);
    httpPOST.setTimeout(25e3);

    //
    while (httpPOST != true)
    {
      Serial.print(F("\t[6/6] Cloud Connection to Soracom cloud, host : "));
      Serial.print(host);
      Serial.print(F(", port : "));
      Serial.print(atoi(prt));

      switch (httpPOST.connect(host, atoi(prt)))
      {
      case true:
        Serial.println(F(" TRUE *"));
        notConnected = false;
        break;
      case false:
        Serial.println(F(" FALSE"));
        break;

      default:
        break;
        Serial.println(F(" switch into default for connect() - err?"));
        break;
      }
      delay(1000);
    }

    if (!httpPOST.connected())
    {
      Serial.println("UXO : Soracom harvest has dropped the connection!..");
      notConnected = true;
      continue;
    }

    char *postBuffer = NULL;

    int payloadLength = snprintf(postBuffer, 0, http_POST_payload, "AT+CSQ", modemReply_1.c_str(), "AT+COPS?", modemReply_2.c_str());
    int postBufferLength = snprintf(postBuffer, 0, http_POST, host, payloadLength, "AT+CSQ", modemReply_1.c_str(), "AT+COPS?", modemReply_2.c_str());

    postBufferLength = postBufferLength + 1; // make space for the NULL char written by snprintf to terminate the string

    postBuffer = (char *)calloc(postBufferLength, sizeof(char));
    snprintf(postBuffer, postBufferLength, http_POST, host, payloadLength, "AT+CSQ", modemReply_1.c_str(), "AT+COPS?", modemReply_2.c_str());

    Serial.print(F("POST BUFFER : "));
    Serial.println(postBuffer);

    httpPOST.beginWrite();
    httpPOST.write((uint8_t *)postBuffer);
    httpPOST.endWrite();

    while (!httpPOST.available())
    {
      Serial.print(F("."));
      delay(1000);
    }

    while (httpPOST.available())
    {
      char byte = static_cast<char>(httpPOST.read());
      Serial.print(byte);
    }

    Serial.println();
    Serial.println(F("client.stop"));
    httpPOST.stop();
    httpGET.stop();

    Serial.print(F("gprs.detachGPRS : "));
    Serial.println(gprs.detachGPRS());

    Serial.print(F("gsmAccess.shutdown : "));
    Serial.println(gsmAccess.shutdown());
  }
}
void loop()
{
  delay(1000);
  Serial.print(F("."));
}