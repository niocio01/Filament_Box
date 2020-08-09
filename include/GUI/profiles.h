#ifndef __PROFILES_H__
#define __PROFILES_H__

#include "Arduino.h"

typedef enum
{
    DESSICANT = 1,
    PLA,
    PETG,
    ABS,
    NYLON,
    PVA,
    TPU,
    ASA,
    PP,
    PC,
    KEEP_DRY,
} profile_Names_t;

typedef struct dry_time_t
{
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
} dry_time_t;

typedef struct profile_t
{
    uint8_t id;
    char name[10];
    uint8_t temperature;
    uint8_t humidity;
    dry_time_t time;
} profile_t;

void profiles_init(void);
void addProfile(profile_t * profile);
profile_t * get_profile(uint8_t id);

profile_t *createProfile (uint8_t id, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours);
// void createProfile(uint8_t index, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_days, uint8_t time_hours, uint8_t time_minutes);


#endif // __PROFILES_H__