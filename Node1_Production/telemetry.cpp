#include "telemetry.h"
#include "network.h"
#include<stdlib.h>

static char* buffer=NULL;   //stores the address of shared json buffer
static uint8_t len=0;

static char* f1(float v,char *dst){  // dtostrf() converts float-->character string
  return dtostrf(v,1,1,dst);  //v-float val,1-min wdth,1-decimal place,dst-destination string
}

void telemetry_init(char* buf, uint8_t bufLen){
buffer=buf;
len=bufLen;
}

void telemetry_publishTelemetry(const SensorData* data, uint8_t relayState){

char t[8],h[8],l[8];  //temp string  , humidty sensor, lm35 string
f1(data->temperature,t);
f1(data->humidity,h);
f1(data->lm35Temp,l);
//put data into json format


// parameters:  buffer->inside which the final json file is stored
//              len->it prevents the overflow
//              PSTR->Format string,template for json 
//              %s--> string placehlder, %u-->unsigned integer placeholder
//               [temp,humidity,lm35]    [vibration,machine status,relay state]

snprintf_P(buffer,len,PSTR("{\"temperature\":%s,\"humidity\":%s,\"lm35\":%s,"
                            "\"vibration\":%u,\"machinestatus\":%u,\"relayState\":%u,"
                            "\"sensorError\":%u}"),t ,h, l,(unsigned)data->vibration,
                            (unsigned)data->machineStatus,(unsigned)relayState,
                            (unsigned)data->sensorError);

Serial.println(buffer);   //prints the generated json to serial monitor

network_publish(TOPIC_TELEMETRY,buffer,0);    //sends telemetry to thingsboard

}