#ifndef GETWEATHER_H
#define GETWEATHER_H
#include "requestTypes.h"

/*
* Gets json file with weather data from specified address using a http request 
*/
int tcpRequest(requestHint_t *hint);


#endif