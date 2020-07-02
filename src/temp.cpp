#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#include <AHT10.h>
#include <Wire.h>

#include "temp.h"
#include <driver/adc.h>

// #define PRINTDATA

TaskHandle_t *ReadTemp_Task_handle;

uint8_t readStatus = 0;

float temperature_AH_1 = 0;
float humitity_AH_1 = 0;

float T_Ptc = 0;


AHT10 myAHT10(AHT10_ADDRESS_0X38);


void initTemp(void) 
{
    // while (myAHT10.begin() != true)
    // {
    //     Serial.println(F("AHT10 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    //     delay(5000);
    // }
    //  Serial.println(F("AHT10 OK"));

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_11db);
    

    xTaskCreate(&ReadTemp_Task, "ReadTemp_Task", 1024, NULL, 5, ReadTemp_Task_handle);
}

void ReadTemp_Task(void *pvParameter) 
{
    esp_task_wdt_add(ReadTemp_Task_handle); //add task to watchdog checking list
    while(1){

        //readStatus = myAHT10.readRawData(); //read 6 bytes from AHT10 over I2C
        long sum = 0;

        for (int i = 0 ; i < 200; i++)
        {
            sum += adc1_get_raw(ADC1_CHANNEL_0); // Pin 36
        }
        
        float Value_Ptc = (float)sum/(float)200;
        float U_Ptc = 3.9/4096*Value_Ptc;
        float R_Ptc = PTC_R1*U_Ptc/(3.3-U_Ptc);
        T_Ptc = PTC_BETA/(log( (R_Ptc)/(PTC_R0*exp(-PTC_BETA/298.15))))-273.15;
        if (T_Ptc == -1)
        {
            Serial.println("error reading thermistor");
        }
         #ifdef PRINTDATA
            else
            {
                Serial.print(Value_Ptc);
                Serial.print(", ");
                Serial.print(U_Ptc);
                Serial.print(", ");
                Serial.println(T_Ptc);
            }
        #endif // PRINTDATA

        

        // if (readStatus != AHT10_ERROR)
        // {
        //     temperature_AH_1 = myAHT10.readTemperature(AHT10_USE_READ_DATA);
        //     humitity_AH_1 = myAHT10.readHumidity(AHT10_USE_READ_DATA);

        //     #ifdef PRINTDATA
        //     Serial.print(F("Temperature: "));
        //     Serial.print(temperature_AH_1);
        //     Serial.print(F("Humidity...: "));
        //     Serial.print(humitity);
        //     Serial.println(F(" +-2%"));
        //     #endif // PRINTDATA
            
        // }
        // else
        // {
        //     Serial.print(F("Failed to read AH10 - reset: "));
        //     // Serial.println(myAHT10.softReset()); //reset 1-success, 0-failed
        // }

        esp_task_wdt_reset(); // reset watchdog
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

float getTemperature(TempSensors_t sensor)
{
    switch (sensor) {
        case THERMISTOR:
            return T_Ptc;
        
        case AHT10_1:
            return temperature_AH_1;

        //case AHT10_2:
        //return temperature_AH_2;

        default:
            return -1;
    }
}

float getHumitity(void)
{
    return humitity_AH_1;
}

void TempProtectionTask(void *pvParameter) 
{
    
}
