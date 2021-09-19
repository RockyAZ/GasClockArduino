#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <TimeLib.h>

AsyncWebServer server(80);

IPAddress    apIP(42, 42, 42, 42);  // Defining a static IP address: local & gateway

const char *ssid = "Steins Yellow Sea";
const char *password = "2283221488";
bool isTimeMode = false;
int hhInt;
int mmInt;
int ssInt;

const char* input_parameter1 = "input_time";
const char* input_parameter2 = "input_number";
const char* input_parameter3 = "input_random";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>HTML Form to Input Data</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem; color: #FF0000;}
  </style>
  </head><body>
  <h2>HTML Form to Input Data</h2> 
  <form action="/get">
    Enter current time[hh.mm.ss]: <input type="text" name="input_time">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter value: <input type="text" name="input_number">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter random deviation value: <input type="text" name="input_random">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void SerringPartTime(String timeStr, int type)
{
  if(type == 0)
  {
    char hh [2];
    hh[0] = timeStr[0];
    hh[1] = timeStr[1];
    hhInt = atoi(hh);
  }
  if(type == 1)
  {
    char mm [2];
    mm[0] = timeStr[3];
    mm[1] = timeStr[4];
    mmInt = atoi(mm);
  }
  if(type == 2)
  {
    char ss [2];
    ss[0] = timeStr[6];
    ss[1] = timeStr[7];
    ssInt = atoi(ss);
  }
}

void SettingTime(String timeStr)
{
  char hh [2];
  hh[0] = timeStr[0];
  hh[1] = timeStr[1];
  hhInt = atoi(hh);
  Serial.println(hhInt);
  char mm [2];
  mm[0] = timeStr[3];
  mm[1] = timeStr[4];
  mmInt = atoi(mm);
  Serial.println(mmInt);
  char ss [2];
  ss[0] = timeStr[6];
  ss[1] = timeStr[7];
  ssInt = atoi(ss);
  Serial.println(ssInt);

//  setTime(hhInt, mmInt, ssInt, 0, 0, 0);
}

void setup() {
  Serial.begin(115200);
  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String input_message;
    String input_parameter;

    if (request->hasParam(input_parameter1)) {
      input_message = request->getParam(input_parameter1)->value();
      input_parameter = input_parameter1;

//      Serial.println("TimeSetted:");
//      Serial.println(input_message);
      if (input_message.length() >= 8)
      {
        isTimeMode = true;
//        SettingTime(input_message);
        SerringPartTime(input_message, 0);
        SerringPartTime(input_message, 1);
        SerringPartTime(input_message, 2);
      }

    }
    else if (request->hasParam(input_parameter2)) {
      input_message = request->getParam(input_parameter2)->value();
      input_parameter = input_parameter2;

      isTimeMode = false;
      Serial.println(input_message);
    }

    else if (request->hasParam(input_parameter3)) {
      input_message = request->getParam(input_parameter3)->value();
      input_parameter = input_parameter3;
      isTimeMode = false;

      char randChar [9];
      randChar[0] = random(3) + '0';
      randChar[1] = ',';
      randChar[2] = random(10) + '0';
      randChar[3] = random(10) + '0';
      randChar[4] = random(10) + '0';
      randChar[5] = random(10) + '0';
      randChar[6] = random(10) + '0';
      randChar[7] = random(10) + '0';
      randChar[8] = '\n';
      Serial.println(randChar);
    }
//    else {
//      input_message = "No message sent";
//      input_parameter = "none";
//    }
//    Serial.println(input_message);
    //    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("+ input_parameter + ") with value: " + input_message + "<br><a href=\"/\">Return to Home Page</a>");
    request->send_P(200, "text/html", index_html);

  });
  server.onNotFound(notFound);
  server.begin();

}

char * properConvertTime(int val)
{
  char res[2];
  if(val < 10)
  {
    res[0] = '0';
    res[1] = val + '0';
  }
  else
    sprintf(res, "%d", val);
  return res;
}

void loop() {
  if(isTimeMode == true)
  {
    delay(1000);
    ssInt++;
    if(ssInt >= 60)
    {
      mmInt++;
      ssInt = 0;
    }
    if(mmInt >= 60)
    {
      hhInt++;
      mmInt = 0;
    }
    if(hhInt >= 24)
      hhInt = 0;

    String timeResult;
    if(hhInt < 10)
      timeResult += '0';
    timeResult += hhInt;
    timeResult += ',';
    if(mmInt < 10)
      timeResult += '0';
    timeResult += mmInt;
    timeResult += ',';
    if(ssInt < 10)
      timeResult += '0';
    timeResult += ssInt;
    timeResult += '\n';
    
//    Serial.printf("%d.%d.%d\n", hhInt, mmInt, ssInt);
  if(isTimeMode == true)
    Serial.println(timeResult);
  }
}
