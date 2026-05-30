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
    float    temperature;      // DHT22 °C
    float    humidity;         // DHT22 %RH
    uint16_t ldrValue;         // Raw ADC (0–1023); lower = darker
    bool     motionDetected;   // PIR: true = motion present
    bool     doorOpen;         // Door sensor: true = door open
    bool     isDark;           // Derived: ldrValue < LDR_DARK_THRESH
    bool     sensorError;      // DHT22 read failure
} SensorData;

// ─────────────────────────────────────────────────────────────
//  SensorManager — POD context struct
//  DHT object is kept as a file-scope static in SensorManager.cpp
// ─────────────────────────────────────────────────────────────

typedef struct {
    float         lastTemp;
    float         lastHumidity;
    bool          motionActive;       // Latched PIR state
    unsigned long motionClearTime;    // When to clear after PIR goes LOW
} SensorManager;

// ── Module functions ──────────────────────────────────────────
void sensor_init  (SensorManager* sm);  //initialises sensorManager variables
void sensor_begin ();    
bool sensors_read  (SensorData* out,SensorManager* sm);  
int  sensor_freeMemory(void);

#endif // SENSOR_MANAGER_H


