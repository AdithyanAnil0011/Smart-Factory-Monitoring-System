#ifndef TELEMETRY_H
#define TELEMETRY_H

/*
 * telemetry.h — builds and publishes JSON payloads to ThingsBoard.
 *
 * Call telemetry_init() once in setup() with the shared JSON buffer.
 * Uses network_publish() internally — no extra dependencies needed.
 */

#include <Arduino.h>
#include "config.h"
#include "SensorManager.h"

void telemetry_init(char* buf, uint8_t bufLen);  //initialise the shared json buffer
                                                //buf-memory buffer address
                                                //bufLen-size of buffer

//build telemetry json and send to cloud                                         
void telemetry_publishTelemetry(const SensorData* data, uint8_t relayState);
                                // data->sensor readings
                                //relayStae-->relay on/off state
                                
// void telemetry_publishAttributes(void);

#endif /* TELEMETRY_H */
