#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <driver/gpio.h>
#include <sdkconfig.h>
#include <Arduino.h>

#include "OTA.h"

#include "blink.h"
#include "temp.h"
#include "heater.h"
#include "webinterface.h"
#include "GUI/display.h"

void main_Task(void *pvParameter);

extern "C" void app_main()
{
    esp_int_wdt_init(); // enable Interrupt watchdog
    esp_task_wdt_init(5, false); // enable Task Watchdog Timer

    initArduino();

    Serial.begin(115200);
    Serial.println("Startup done!");

    setupOTA("FilamentBox");

    // initBlink();
    //initServer();
    initTemp();
    //initHeater();
    display_init();

    xTaskCreate(&main_Task, "main_Task", 1024*4, NULL, 5, NULL);
    
}

void main_Task(void *pvParameter)
{
    while(1)
    {
        ArduinoOTA.handle();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}