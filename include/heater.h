#ifndef __HEATER_H__
#define __HEATER_H__

// safety parameters
#define MINTEMP 0
#define MAXTEMP 60
#define MAX_HEATING_TIME 5000 // maximum allowed time in ms to heat One Degree


#define PWM_MIN_DUTY 10 // of 255.
#define PWM_MAX_DUTY 255 // of 255


// PID Terms
#define KP 40 // Proportional
#define KD 0 // Derivative
#define KI 0 // Integral

#define HEATER_PIN 16
#define HEATER_CHANNEL 0
#define HEATER_SWITCHING_FRQ 10
#define HEATER_RESOLUTION 8

void initHeater(void);
void heaterPID_Task(void *pvParameter);
void set_Goal(float temperature);
void startHeater(void);
void stopHeater(void);



#endif // __HEATER_H__