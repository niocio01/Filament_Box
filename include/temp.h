#ifndef __TEMP_H__
#define __TEMP_H__

// Ptc Calculations Parameters
#define PTC_BETA 4261
#define PTC_R1 100000 // resistance of resistor in series of PTC
#define PTC_R0 100000 // resistance of PTC at refference Temperature PTC_T0
#define PTC_T0 25 + 273.15

typedef enum
{
    THERMISTOR,
    AHT10_1,
    AHT10_2,
    ESP32_TEMP_SENS
} TempSensors_t;

void initTemp(void);
void ReadTemp_Task(void *pvParameter);
void TempProtectionTask(void *pvParameter);

void readTemperature_Thermister(void);
void readTemperature_AH10_1(void);
void readTemperature_ESP32(void);

float getTemperature(TempSensors_t sensor);
float getHumitity(void);

#endif // __TEMP_H__