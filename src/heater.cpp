#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#include "heater.h"
#include "temp.h"

TaskHandle_t *HeaterPID_Task_handle;

#define PRINTINFO

float temp_goal = 0;
float setpoint_sum = 0;
float setpoint_old = 0;

int dutyOut = 0;
bool allowHeating = false;

void initHeater(void) 
{
    ledcSetup(HEATER_CHANNEL, HEATER_SWITCHING_FRQ, HEATER_RESOLUTION);
    ledcAttachPin(HEATER_PIN, HEATER_CHANNEL);

    xTaskCreate(&heaterPID_Task, "HeaterPID_Task", 1024, NULL, 5, HeaterPID_Task_handle);

    set_Goal(30.00);
    startHeater();
}

void heaterPID_Task(void *pvParameter) 
{
    esp_task_wdt_add(HeaterPID_Task_handle); //add task to watchdog checking list
    while(1){

        float currentTemp = getTemperature(ESP32_TEMP_SENS);
        if (currentTemp > MAXTEMP) {
            Serial.print("ERROR Maxtemp!, shutting down...");
            stopHeater(); }

        else if (currentTemp < MINTEMP) {
            Serial.print("ERROR Mintemp!, shutting down...");
            stopHeater(); }
        
        else if (currentTemp == -1){
            Serial.print("ERROR Thermistor reading!, shutting down...");
            stopHeater(); }

        float setpoint = temp_goal - currentTemp; // calculate setpoint

        dutyOut = (int)(KP * setpoint); // calculate dutycycle

        if (dutyOut > PWM_MAX_DUTY)
        {
            dutyOut = PWM_MAX_DUTY;
        }
        else if(dutyOut < PWM_MIN_DUTY)
        {
            dutyOut = PWM_MIN_DUTY;
        }

        if (allowHeating)
        {
            ledcWrite(HEATER_CHANNEL, dutyOut);
        }
        else
        {
            ledcWrite(HEATER_CHANNEL, 0);
        }

        #ifdef PRINTINFO
            Serial.print("\nHeater is: ");
            allowHeating ? Serial.print("ON ") : Serial.print("OFF");
            Serial.print("    C_Temp: ");
            Serial.print(currentTemp);
            Serial.print("    S_Temp: ");
            Serial.print(temp_goal);
            Serial.print("    Setpoint: ");
            Serial.print(setpoint);
            Serial.print("    DutyCycle: ");
            Serial.print(dutyOut);
        #endif // PRINTINFO

        esp_task_wdt_reset(); // reset watchdog
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

void set_Goal(float temperature) 
{
    temp_goal = temperature;
}

void startHeater(void) 
{
    allowHeating = true;
}

void stopHeater(void) 
{
    allowHeating = false;
}
