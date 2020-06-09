#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#include <AHT10.h>
#include <Wire.h>

#include "temp.h"

TaskHandle_t *ReadTemp_Task_handle;

uint8_t readStatus = 0;
AHT10 myAHT10(AHT10_ADDRESS_0X38);


void initTemp(void) 
{
    while (myAHT10.begin() != true)
    {
        Serial.println(F("AHT10 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
        delay(5000);
    }
     Serial.println(F("AHT10 OK"));

    xTaskCreate(&ReadTemp_Task, "ReadTemp_Task", configMINIMAL_STACK_SIZE, NULL, 5, ReadTemp_Task_handle);
}

void ReadTemp_Task(void *pvParameter) 
{
    esp_task_wdt_add(ReadTemp_Task_handle); //add task to watchdog checking list
    while(1){

        readStatus = myAHT10.readRawData(); //read 6 bytes from AHT10 over I2C

        if (readStatus != AHT10_ERROR)
        {
            Serial.print(F("Temperature: "));
            Serial.print(myAHT10.readTemperature(AHT10_USE_READ_DATA));
            Serial.println(F(" +-0.3C"));
            Serial.print(F("Humidity...: "));
            Serial.print(myAHT10.readHumidity(AHT10_USE_READ_DATA));
            Serial.println(F(" +-2%"));
        }
        else
        {
            Serial.print(F("Failed to read - reset: "));
            Serial.println(myAHT10.softReset()); //reset 1-success, 0-failed
        }

        esp_task_wdt_reset(); // reset watchdog
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void TempProtectionTask(void *pvParameter) 
{
    
}
