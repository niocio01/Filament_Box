#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"
#include "blink.h"

TaskHandle_t *blink_handle;


bool state = false;


void initBlink(void)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);

    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, blink_handle);
    
}

void blink_task(void *pvParameter)
{
    esp_task_wdt_add(blink_handle); //add task to watchdog checking list
    while(1) {
        gpio_set_level(BLINK_GPIO, state);
        state = !state;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        esp_task_wdt_reset(); // reset watchdog
        // Serial.print(state);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        
    }
}
