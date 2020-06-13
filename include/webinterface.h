#ifndef __WEBINTERFACE_H__
#define __WEBINTERFACE_H__

#include <WString.h>

void initServer(void);
void server_Task(void *pvParameter);

void handle_OnConnect(void);
void handleSensorData(void);
String SendHTML(void);

#endif // __WEBINTERFACE_H__