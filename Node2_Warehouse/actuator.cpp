#include "actuator.h"

static uint8_t relay_status;

static uint8_t blinkstate=0;
static unsigned long lastBlinkTime=0;



void    actuators_begin(void){
  
   pinMode(PIN_LED_GREEN,OUTPUT);   //IF its on everything is normal
    pinMode(PIN_LED_YELLOW,OUTPUT);  //If on ->warning,blinking means critical
     pinMode(PIN_LED_RED,OUTPUT);   //not connected
   pinMode(PIN_RELAY,OUTPUT);//ventilation ,if more humidity

     digitalWrite(PIN_LED_GREEN,LOW); 
     digitalWrite(PIN_LED_YELLOW,LOW);  
     digitalWrite(PIN_RELAY,LOW);    

     

}

void    actuators_setRelay(uint8_t on){
   
   relay_status=on;
   digitalWrite(PIN_RELAY,on ? HIGH:LOW);

}
uint8_t actuators_getRelayState(void){
   return relay_status;

}

void    actuators_updateStatusLEDs(uint8_t mqttConnected, uint8_t sensorError){
// if board not connected to thingsboard then turn on red led
if(!mqttConnected){
   digitalWrite(PIN_LED_RED,HIGH);
   digitalWrite(PIN_LED_GREEN,LOW);
   digitalWrite(PIN_LED_YELLOW,LOW);
   return;
}

//if mqtt is up then turn off the red led
digitalWrite(PIN_LED_RED,LOW);
//IF there is any sensor error turn on yellow led
if(sensorError){
   digitalWrite(PIN_LED_GREEN,HIGH);
   unsigned long now=millis();
   if(now - lastBlinkTime >500){  //delay of 0.5 sec
   lastBlinkTime=now;
   blinkstate=!blinkstate;
   digitalWrite(PIN_LED_YELLOW,blinkstate ? HIGH:LOW);
   
   }
}
   else{
      digitalWrite(PIN_LED_GREEN,HIGH);  //if no errors
       digitalWrite(PIN_LED_YELLOW,LOW);
   }
   
}