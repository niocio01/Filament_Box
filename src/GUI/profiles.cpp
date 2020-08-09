#include <string.h>
#include "profiles.h"


<<<<<<< HEAD

=======
>>>>>>> b5bcbfe1ab0c3a9430a8b83da6b3ee5716aae6f8
void profiles_init(void)
{
    addProfile(createProfile(DESSICANT, "Dessicant", 65, 0, 3));
    addProfile(createProfile(PLA, "PLA", 45, 0, 4));
    addProfile(createProfile(ABS, "ABS", 60, 0, 2));
    addProfile(createProfile(PETG, "PETG", 65, 0, 2));
    addProfile(createProfile(NYLON, "Nylon", 70, 0, 12));
    addProfile(createProfile(PVA, "PVA", 45, 0, 4));
    addProfile(createProfile(TPU, "TPU-R", 50, 0, 4));
    addProfile(createProfile(ASA, "ASA", 60, 0, 4));
    addProfile(createProfile(PP, "PP", 55, 0, 6));
    addProfile(createProfile(PC, "PC", 70, 0, 10));

    for (int i = 0; i < 10; i++)
    {
        char str[10];
        Serial.print(i);
        Serial.print(": ");
        Serial.print(profiles[i]->name);
        Serial.print(" @ ");
        Serial.print(profiles[i]->temperature);
    }
}


int addProfile(profile_t * profile)
{
    static uint8_t noOfProfiles = 0;

    if (noOfProfiles == 0)
    {
        
    }
    else
    {
        int i = 0;
        while (profiles[i]->temperature < profiles[i+1]->temperature)
        {
            i++;
        }
        profiles[i] = profile;
    }

}

profile_t *createProfile (uint8_t id, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours)
{
    profile_t *p = (profile_t*)malloc(sizeof(profile_t));

    strcpy(p->name, name);
    p->temperature = temperature;
    p->humidity = humidity;
    p->time.minutes = 0;
    p->time.hours = time_hours;
    p->time.days = 0;

    return p;
}