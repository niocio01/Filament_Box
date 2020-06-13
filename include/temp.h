#ifndef __TEMP_H__
#define __TEMP_H__

#define MaxTemp 90
#define MinTemp 0
#define maxHeatingTime 5 // maximum allowed time to heat One Degree

void initTemp(void);
void ReadTemp_Task(void *pvParameter);
void TempProtectionTask(void *pvParameter);

float getTemperature(void);
float getHumitity(void);

#endif // __TEMP_H__