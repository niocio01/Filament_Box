#include <string.h>
#include "profiles.h"

struct profile_t * profilesByTemperature[20];
struct profile_t * profilesByID[20];

uint8_t noOfProfiles = 0;

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
        Serial.print(profilesByTemperature[i]->name);
        Serial.print("(");
        Serial.print(profilesByTemperature[i]->id);
        Serial.print(")\t@ ");
        Serial.print(profilesByTemperature[i]->temperature);
        Serial.print("°C for ");
        Serial.print(profilesByTemperature[i]->time.hours);
        Serial.println(" hours");
    }
    Serial.println("");

    profilesByID[CUSTOM] = profiles_createProfile(CUSTOM, "custom", 0, 100, 0);

}


void profiles_addProfile(profile_t * profile)
{
    if (noOfProfiles == 0)
    {
        profilesByTemperature[0] = profile;
    }
    else
    {
        int insertPoint = 0;
        for (int i = 0; i < noOfProfiles; i++)
        {
            if(profilesByTemperature[i]->temperature < profile->temperature)
            {
                insertPoint++;
            }
        }
        for (int j = noOfProfiles+1; j > insertPoint; j--)
        {
            profilesByTemperature[j] = profilesByTemperature[j-1];
        }
        profilesByTemperature[insertPoint] = profile;
        
    }
    profilesByID[profile->id] = profile;

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


void profiles_setCurrentProfile(uint8_t presetId)
{
    profilesByID[CUSTOM]->id = presetId;
    profilesByID[CUSTOM]->temperature = profilesByID[presetId]->temperature;
    profilesByID[CUSTOM]->humidity = profilesByID[presetId]->humidity;
    profilesByID[CUSTOM]->time.days = profilesByID[presetId]->time.days;
    profilesByID[CUSTOM]->time.hours = profilesByID[presetId]->time.hours;
    profilesByID[CUSTOM]->time.minutes = profilesByID[presetId]->time.minutes;

    char str1[40];
    char str2[40];

    strcpy(str1, profilesByID[profilesByID[CUSTOM]->id]->name);
    strcpy(str2, " *");
    strcat(str1, str2);

    strcpy(profilesByID[CUSTOM]->name, str1);
}

bool profiles_Compare_CurrentToPreset(void)
{
    if(profilesByID[CUSTOM]->temperature != profilesByID[profilesByID[CUSTOM]->id]->temperature) {return false;}
    if(profilesByID[CUSTOM]->humidity != profilesByID[profilesByID[CUSTOM]->id]->humidity) {return false;}
    if(profilesByID[CUSTOM]->time.days != profilesByID[profilesByID[CUSTOM]->id]->time.days) {return false;}
    if(profilesByID[CUSTOM]->time.hours != profilesByID[profilesByID[CUSTOM]->id]->time.hours) {return false;}
    if(profilesByID[CUSTOM]->time.minutes != profilesByID[profilesByID[CUSTOM]->id]->time.minutes) {return false;}

    return true;
}