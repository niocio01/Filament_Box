#include <string.h>
#include "profiles.h"

profile_t profiles[20] = {0};

void profiles_init(void)
{
    createProfile(DESSICANT, "Dessicant", 65, 0, 3);
    createProfile(PLA, "PLA", 45, 0, 4);
    createProfile(ABS, "ABS", 60, 0, 2);
    createProfile(PETG, "PETG", 65, 0, 2);
    createProfile(NYLON, "Nylon", 70, 0, 12);
    createProfile(PVA, "PVA", 45, 0, 4);
    createProfile(TPU, "TPU-R", 50, 0, 4);
    createProfile(ASA, "ASA", 60, 0, 4);
    createProfile(PP, "PP", 55, 0, 6);
    createProfile(PC, "PC", 70, 0, 10);
}

void createProfile (uint8_t index, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours)
{
    strcpy(profiles[index].name, name);
    profiles[index].temperature = temperature;
    profiles[index].humidity = humidity;
    profiles[index].time.minutes = 0;
    profiles[index].time.hours = time_hours;
    profiles[index].time.days = 0;
}

void createProfile (uint8_t index, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_days, uint8_t time_hours, uint8_t time_minutes)
{
    strcpy(profiles[index].name, name);
    profiles[index].temperature = temperature;
    profiles[index].humidity = humidity;
    profiles[index].time.minutes = time_minutes
    profiles[index].time.hours = time_hours;
    profiles[index].time.days = time_days;
}