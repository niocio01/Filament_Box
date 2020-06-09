/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"
#include <Arduino.h>
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO (gpio_num_t)CONFIG_BLINK_GPIO
#define LED_BUILTIN 4

TaskHandle_t *blink_handle;

void blink_task(void *pvParameter)
{
    esp_task_wdt_add(blink_handle); //add task to watchdog checking list
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        esp_task_wdt_reset(); // reset watchdog
    }
}

void arduinoTask(void *pvParameter) {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    while(1) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        Serial.println("Hello!");
        delay(1000);
    }
}

extern "C" void app_main()
{
    esp_int_wdt_init(); // enable Interrupt watchdog
    esp_task_wdt_init(5, false); // enable Task Watchdog Timer

    // initialize arduino library before we start the tasks
    initArduino();

    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, blink_handle);
    xTaskCreate(&arduinoTask, "arduino_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}