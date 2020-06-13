#include "webinterface.h"
#include "blink.h"

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include "credentials.h"
#include "temp.h"

const char* ssid = mySSID;
const char* password = myPASSWORD;

IPAddress local_ip(192,168,1,37);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

TaskHandle_t *Server_Task_handle;

String ledState;

AsyncWebServer server(80);

void initServer(void)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting to WiFi..");
    }

    Serial.println("");
    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP());
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false, handleSensorData);
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.begin();
    Serial.println("HTTP server started");

    // xTaskCreate(&server_Task, "Server_Task", 1024 * 50, NULL, 5, Server_Task_handle);
}


// Replaces placeholder with LED state value
String handleSensorData(const String& var){
    char temp[10], hum[10];
    dtostrf(getTemperature(), 4, 2, temp);
    dtostrf(getHumitity(), 4, 2, hum);
  Serial.println(temp);

  if(var == "STATE"){
    if(digitalRead(LED_BUILTIN)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}


String SendHTML(void)
{

    String ptr = "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>";
    ptr +="<title>Filament Box</title>\n";
    // ptr +="<meta http-equiv=\"refresh\" content=\"5\">";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr +=".button-on {background-color: #3498db;}\n";
    ptr +=".button-on:active {background-color: #2980b9;}\n";
    ptr +=".button-off {background-color: #34495e;}\n";
    ptr +=".button-off:active {background-color: #2c3e50;}\n";
    ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<h1>Filament Box</h1>\n";
    ptr +="<h3>Controll</h3>\n";

    ptr +="<p>Temperature:</p>&nbsp;<span id='temp'></span>";

    ptr +="</body>\n";
    ptr +="</html>\n";

    ptr += "<script>\n";
    ptr += "$(function() {\n";
    ptr += "setInterval(requestData, 1000);\n";
    ptr += "function requestData() {\n";
    ptr += "$.get(\"/sensors\")\n";
    ptr += ".done(function(data) {\n";
    ptr += "console.log(data);\n"; //debuging
    ptr += "if (data) {\n";
    ptr += "$(\"#temp\").text(data.temp);\n";
    ptr += "$(\"#pin39\").text(data.pin39);\n";
    ptr += "$(\"#pin5\").text(data.pin5);\n";
    ptr += "} else {\n";
    ptr += "$(\"#temp\").text(\"?\");\n";
    ptr += "$(\"#pin39\").text(\"?\");\n";
    ptr += "$(\"#pin5\").text(\"?\");\n";
    ptr += "}\n";
    ptr += "}).fail(function() {\n";
    ptr += "console.log(\"The was a problem retrieving the data.\");\n";
    ptr += "});\n";
    ptr += "}\n";
    ptr += "});\n";
    ptr += "</script>\n";

    return ptr;
}