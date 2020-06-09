#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <driver/gpio.h>
#include <sdkconfig.h>
#include <Arduino.h>

#include "blink.h"
#include "temp.h"


extern "C" void app_main()
{
    esp_int_wdt_init(); // enable Interrupt watchdog
    esp_task_wdt_init(5, false); // enable Task Watchdog Timer

    initArduino();

    Serial.begin(115200);
    Serial.println("Startup done!");

    initBlink();
    initTemp();
}