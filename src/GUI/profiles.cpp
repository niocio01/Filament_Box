#include <string.h>
#include "profiles.h"

struct profile_t * profiles[20];

uint8_t noOfProfiles = 0;

uint8_t selectedProfileId;
profile_t * currentProfile;

void profiles_init(void)
{
    profiles_addProfile(profiles_createProfile(DESSICANT, "Dessicant", 65, 0, 3));
    profiles_addProfile(profiles_createProfile(PLA, "PLA", 45, 0, 4));
    profiles_addProfile(profiles_createProfile(ABS, "ABS", 60, 0, 2));
    profiles_addProfile(profiles_createProfile(PETG, "PETG", 65, 0, 2));
    profiles_addProfile(profiles_createProfile(NYLON, "Nylon", 70, 0, 12));
    profiles_addProfile(profiles_createProfile(PVA, "PVA", 45, 0, 4));
    profiles_addProfile(profiles_createProfile(TPU, "TPU", 50, 0, 4));
    profiles_addProfile(profiles_createProfile(ASA, "ASA", 60, 0, 4));
    profiles_addProfile(profiles_createProfile(PP, "PP", 55, 0, 6));
    profiles_addProfile(profiles_createProfile(PC, "PC", 70, 0, 10));
    profiles_addProfile(profiles_createProfile(KEEP_DRY, "Keep Dry", 0, 0, 0));

    Serial.print("\nAdded " );
    Serial.print(noOfProfiles);
    Serial.println(" Profiles:");

    for (int i = 0; i < noOfProfiles; i++)
    {
        Serial.print(i+1);
        Serial.print(".: ");
        Serial.print(profiles[i]->name);
        Serial.print("(");
        Serial.print(profiles[i]->id);
        Serial.print(")\t@ ");
        Serial.print(profiles[i]->temperature);
        Serial.print("°C for ");
        Serial.print(profiles[i]->time.hours);
        Serial.println(" hours");
   }
   Serial.println("");

    selectedProfileId = 0;
    currentProfile = profiles_createProfile(CUSTOM, "custom", 0, 100, 0);
   
}


void profiles_addProfile(profile_t * profile)
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

profile_t * profiles_createProfile (uint8_t id, char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_hours)
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

profile_t * profiles_getProfile(uint8_t id)
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


void profiles_setCurrentProfile_byValues(char name[10], uint8_t temperature, uint8_t humidity, uint8_t time_days, uint8_t time_hours, uint8_t time_minutes)
{
    currentProfile->id = 0;
    currentProfile->temperature = temperature;
    currentProfile->humidity = humidity;
    currentProfile->time.days = time_days;
    currentProfile->time.hours = time_hours;
    currentProfile->time.minutes = time_minutes;
}

void profiles_setCurrentProfile_byPreset(uint8_t presetId)
{
    selectedProfileId = presetId;
    currentProfile->id = presetId;
    strcpy(currentProfile->name, profiles_getProfile(presetId)->name);
    currentProfile->temperature = profiles_getProfile(presetId)->temperature;
    currentProfile->humidity = profiles_getProfile(presetId)->humidity;
    currentProfile->time.days = profiles_getProfile(presetId)->time.days;
    currentProfile->time.hours = profiles_getProfile(presetId)->time.hours;
    currentProfile->time.minutes = profiles_getProfile(presetId)->time.minutes;
}

bool profiles_Compare_CurrentToPreset(void)
{
    if(currentProfile->temperature != profiles_getProfile(selectedProfileId)->temperature) return false;
    if(currentProfile->humidity != profiles_getProfile(selectedProfileId)->humidity) return false;
    if(currentProfile->time.days != profiles_getProfile(selectedProfileId)->time.days) return false;
    if(currentProfile->time.hours != profiles_getProfile(selectedProfileId)->time.hours) return false;
    if(currentProfile->time.minutes != profiles_getProfile(selectedProfileId)->time.minutes) return false;

    return true;
}