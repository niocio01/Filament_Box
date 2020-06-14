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
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/scripts.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/scripts.js", "text/js");
    });

   
   server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/sensordata.json", String(), false, handleSensorData);

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

  if(var == "TEMP"){
    return F(temp);
  }
  if(var == "HUM"){
    return F(hum);
  }
  return String();
}