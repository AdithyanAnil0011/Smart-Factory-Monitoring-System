#include "SensorManager.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);     //creates a dht object

void sensor_init  (SensorManager* sm){
  sm->lastTemp=0.0f;
  sm->lastHumidity=0.0f;
  sm->motionActive=false;
  sm->motionClearTime=0;

}

// static float lastTemp;
// static float lastHumidity;

void sensor_begin(void)
{
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DOOR, INPUT_PULLUP);
  dht.begin();

}  
//read sensor of pir
static bool readPIR(SensorManager* sm){
  //read the value from motion sensor ,check if it is true for next 10 sec
  const bool pirHigh=(digitalRead(PIN_PIR)==HIGH);
  const unsigned long now=millis();

  if(pirHigh){  // if motion detected
    sm->motionActive=true;
    sm->motionClearTime=now + MOTION_CLEAR_MS;

  }
  else if(sm->motionActive && (now >=sm->motionClearTime)){
     sm->motionActive=false; 
  }

  return sm->motionActive;  //1->for motion   and 0->for no motion
}

//read door sensor status
static bool readDoor(void){
  return (digitalRead(PIN_DOOR)==LOW);   //0 when door is open and 1 when the door is closed
}

//read motion detection


//read ldr sensor
static uint16_t readLDR(void){
  uint16_t sum=0;
  uint8_t i;

  for(i=0;i<4;i++){             //avg of 4 readings
    sum+=(uint16_t)analogRead(PIN_LDR);   // 0 to 1023
    delay(2);
  }
  return sum/4;     //wear house is dark or not,300>val->dark light is available 
}

//read data from sensor and return status of dht22->return 0 on error

bool  sensors_read(SensorData* out,SensorManager* sm){
  //read dht22 sensor
  float t=dht.readTemperature();
  float h=dht.readHumidity();


  if(isnan(t)|| isnan(h)){  //    if there is some dht failure
  
     out->sensorError=1;   //sets error flag
    out->temperature=sm->lastTemp;
    out->humidity=sm->lastHumidity;
  }
  else{                       //if no dht failure
     out->sensorError=0;
    out->temperature=round(t* 10.0f)*0.1f;   //rounds of 1 decimal place
    out->humidity=round(h* 10.0f)*0.1f;
    sm->lastTemp=out->temperature;
    sm->lastHumidity=out->humidity;

  }

  
  //read ldr sensor/ door status/read motion detected

  out->motionDetected=readPIR(sm);     //reads the motion state
  out->doorOpen=readDoor();            
  out->ldrValue=readLDR();
  out->isDark=(out->ldrValue <LDR_DARK_THRESH); 



  return !out->sensorError;


  //read lm35

}
