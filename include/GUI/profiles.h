#ifndef __PROFILES_H__
#define __PROFILES_H__

#include "Arduino.h"

typedef enum
{
    CUSTOM = 0,
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

extern struct profile_t * profilesByTemperature[20];
extern struct profile_t * profilesByID[20];

void profiles_init(void);
void profiles_addProfile(profile_t * profile);

profile_t * profiles_createProfile (uint8_t id, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours);
// void createProfile(uint8_t index, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_days, uint8_t time_hours, uint8_t time_minutes);

void profiles_setCurrentProfile(uint8_t presetId);

bool profiles_Compare_CurrentToPreset(void);



#endif // __PROFILES_H__