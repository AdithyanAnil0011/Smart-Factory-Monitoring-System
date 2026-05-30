#include "actuator.h"


static uint8_t blinkstate=0;     //used for blinking led
static unsigned long lastBlinkTime=0;
static uint8_t relayStatus;       // tells if relay is on / off
void    actuators_begin(void){
  
   pinMode(PIN_LED_GREEN,OUTPUT);   //connected
    pinMode(PIN_LED_YELLOW,OUTPUT);  //warning
     pinMode(PIN_LED_RED,OUTPUT);   //not connected

     pinMode(PIN_RELAY,OUTPUT);

     digitalWrite(PIN_LED_RED,HIGH);   //In the begining it is not connected
     

}

void  actuators_setRelay(uint8_t on){     //controls the relay
   relayStatus=on;
   digitalWrite(PIN_RELAY,on ? HIGH:LOW); //equivalent to 
                                          //  if(on) digitalWrite(PIN_RELAY,HIGH);
                                          //   else
                                          //  digitalWrite(PIN_RELAY,LOW);

}

//sent back the status of the relay to the things board(ir current relay status)
uint8_t actuators_getRelayState(void){
   return relayStatus;

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
   unsigned long now=millis();  //millis() used instead of delay(),it allows mqtt,sensor ,dashboard etc working
                                 // along with led blinking
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


//if everything is normal turn on green led
//if sensor eror blink yellow led
