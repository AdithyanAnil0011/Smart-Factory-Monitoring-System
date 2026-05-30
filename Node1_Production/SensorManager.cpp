#include "SensorManager.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);//Creates an internal sensor driver engine instance named dht

static float lastTemp;
static float lastHumidity;

void sensor_begin(void)
{
  pinMode(PIN_BUTTON, INPUT_PULLUP);//Configures Digital Pin 3 as an input and switches on the microcontroller’s internal resistor
  dht.begin();             //Enables pull up resistor default state=High and pressed state=Low
} 

//read vibration from POT
static uint16_t readVibration(void){
  uint16_t sum=0;
  uint8_t i;
  for(i=0;i<4;i++){
    sum+=analogRead(PIN_VIBRATION);  //Averaging them (instead of taking 1 unstable reading)
    delay(100);                            // (improves the stability)
  }
  return sum/4;   //return avg of 4 samples

}

//read lm35 temp
static float readLM35(void){
  //read temp from lm35 convert to celcius
  float t=analogRead(PIN_LM35)*(500.0f/1023.0f); // conversion of 0-1023 val into celcius
  return round(t*10.0f)*0.1f;   //rounding off to 1 decimal place(improves readability)


}


//read data from sensor and return status of dht22->return 0 on error
uint8_t sensors_read(SensorData* out){  
  //read dht22 sensor
  float t=dht.readTemperature();
  float h=dht.readHumidity();

  // if error in dht sensor 
  if(isnan(t)|| isnan(h)){  //    if there is some dht failure
    out->sensorError=1;   //sets error flsg
    out->temperature=lastTemp;
    out->humidity=lastHumidity;
  }
  else{                       //if no dht failure
     out->sensorError=0;
    out->temperature=round(t* 10.0f)*0.1f;   //rounds of 1 decimal place
    out->humidity=round(h* 10.0f)*0.1f;
    lastTemp=out->temperature;
    lastHumidity=out->humidity;

  }

  
  //read vibration sensor
  out->lm35Temp=readLM35();
  out->vibration=readVibration();
  out->machineStatus=(digitalRead(PIN_BUTTON)==LOW)? 1:0;

  return out->sensorError;



}