#include <string.h>
#include "profiles.h"


struct profile_t * profiles[20] ;
uint8_t noOfProfiles = 0;

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
    addProfile(createProfile(KEEP_DRY, "Keep Dry", 0, 0, 0));

   for (int i = 0; i < noOfProfiles; i++)
   {
       Serial.print(i);
       Serial.print(": ");
       Serial.print(profiles[i]->name);
       Serial.print("(");
       Serial.print(profiles[i]->id);
       Serial.print(") @ ");
       Serial.println(profiles[i]->temperature);
   }
}


void addProfile(profile_t * profile)
{
    if (noOfProfiles == 0)
    {
        profiles[0] = profile;
    }
    else
    {
        int insertPoint = 0;
        for (int i = 0; i < noOfProfiles; i++)
        {
            if(profiles[i]->temperature < profile->temperature)
            {
                insertPoint++;
            }
        }
        for (int j = noOfProfiles+1; j > insertPoint; j--)
        {
            profiles[j] = profiles[j-1];
        }
        profiles[insertPoint] = profile;
        
    }

    noOfProfiles++;
}

profile_t *createProfile (uint8_t id, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours)
{
    profile_t *p = (profile_t*)malloc(sizeof(profile_t));

    strcpy(p->name, name);
    p->id = id;
    p->temperature = temperature;
    p->humidity = humidity;
    p->time.minutes = 0;
    p->time.hours = time_hours;
    p->time.days = 0;

  //  Serial.print(p->name);
  //  Serial.println(" added");

    return p;
}

profile_t * get_profile(uint8_t id)
{
    for (int i = 0; i < noOfProfiles; i++)
    {
        if (profiles[i]->id == id)
        {
            return profiles[i];
        }
    }
    // should never get here
    return 0;
}