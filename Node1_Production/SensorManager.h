#ifndef SENSORS_H
#define SENSORS_H

/*
 * sensors.h — reads DHT22, LM35, vibration sensor, and button.
 *
 * C-style module: all state lives in static globals inside sensors.cpp.
 * Call sensors_begin() once in setup(), then sensors_read() in loop().
 */

#include <Arduino.h>
#include "config.h"

/* All sensor readings packed into one plain struct */
typedef struct {
    float    temperature;   /* DHT22 temperature in °C        */
    float    humidity;      /* DHT22 humidity in %            */
    float    lm35Temp;      /* LM35 temperature in °C         */
    uint16_t vibration;     /* averaged ADC reading (0–1023)  */
    uint8_t  machineStatus; /* 1 = button pressed (active LOW) */
    uint8_t  sensorError;   /* 1-byte flag,  1 = DHT22 read failed          */
} SensorData;

void    sensor_begin(void);    //This initializes your sensor hardware.
uint8_t sensors_read(SensorData* out);   /* It will run inside your main loop() function every 5 seconds. returns 0 on DHT error */
int     sensors_freeMemory(void);        /*It calculates how many bytes of free SRAM in bytes     */

#endif /* SENSORS_H */
