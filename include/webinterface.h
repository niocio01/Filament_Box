#ifndef __WEBINTERFACE_H__
#define __WEBINTERFACE_H__

#include <WString.h>

void initServer(void);
void server_Task(void *pvParameter);

void handle_OnConnect(void);
String handleSensorData(const String& var);
String SendHTML(void);

#endif // __WEBINTERFACE_H__