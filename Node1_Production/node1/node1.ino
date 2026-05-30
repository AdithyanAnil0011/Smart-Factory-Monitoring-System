//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"
#include "network.h"
#include "telemetry.h"
#include "actuator.h"
#include "rpc.h"
static char sharedBuf[200];    //shared json memory buffer,used by telemetry module,rpc module
//instead of separate buffer for telemetry and rpc,we use same buffer

//create object of type ethernet pubsubclient
static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);   //creates a mqtt client

unsigned long lastTelemetry=0;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Node 1 is booting");

  // put your setup code here, to run once:

   // initilase sensor 
   sensor_begin();

   //initialise actutors
   actuators_begin();

   // coonect board to internet and mqtt

   telemetry_init(sharedBuf,sizeof(sharedBuf));  //Telemetry initialisation

   rpc_init(&mqttClient,sharedBuf,sizeof(sharedBuf));  //initialising rpc request
   
   //upon receiving the data which func needs to be called back
      mqttClient.setCallback(rpc_mqttCallback);

   network_begin(&mqttClient);

   digitalWrite(PIN_LED_RED,LOW);

   
}

void loop() 
{
  // put your main code here, to run repeatedly:
//keep checking if board connected to things cloud
network_maintain();

//read data from the sensors 
SensorData data;       //creates a sensor data container

sensors_read(&data);   // humidity,temp,lm35,vibratrion,sensor_error,machine_status

  //publish to the cloud 
  //check network available then push data to things board

if(network_isConnected()){
    unsigned long now=millis();
     //publish data to thingsboard every 5 sec

     if(now-lastTelemetry >=TELEMETRY_INTERVAL){  // if 5 sec is over
      lastTelemetry=now;
      telemetry_publishTelemetry(&data,actuators_getRelayState());  //converting sensor into json and publish
     }
  }


 actuators_updateStatusLEDs(network_isConnected(),data.sensorError);
 //controls the red,green and yellow leds
 }
 




















