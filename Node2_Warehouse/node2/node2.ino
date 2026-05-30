//include required libraries
#include <SPI.h>         //needed for w5100 ethernet shield
#include <Ethernet.h>
#include <PubSubClient.h>
#include "SensorManager.h"
#include <DHT.h>

#include "config.h"

#include "network.h"
#include "telemetry.h"
#include "actuator.h"
#include "rpc.h"



static char sharedBuf[200];          //used by telemetry and rpc

//create object of type ethernet pubsubclient
static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);

unsigned long lastTelemetry=0;
static SensorManager  sensors;
static bool autoTriggered = false;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Node 2 is booting");

  // put your setup code here, to run once: 

   // initilase sensor 
   sensor_begin();

   sensor_init(&sensors);

   //initialise actutors
   actuators_begin();

   // coonect board to internet and mqtt

   telemetry_init(sharedBuf,sizeof(sharedBuf));
   rpc_init(&mqttClient,sharedBuf,sizeof(sharedBuf)); 
   mqttClient.setCallback(rpc_mqttCallback);
   network_begin(&mqttClient);

  //  digitalWrite(PIN_LED_RED,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:

//keep checking if board connected to things cloud
network_maintain();

  //read data from the sensors 
SensorData data;
sensors_read(&data, &sensors);  // humidity,temp,ldr,door status,motion status

// if humidity is more than 85 % turn on the relay if relay is off

// if(data.humidity >=HUMIDITY_CRIT && !actuators_getRelayState()){
//   actuators_setRelay(true); 

// }
// Auto turn ON ventilation once when humidity becomes critical
if(data.humidity >= HUMIDITY_CRIT && !autoTriggered){
    actuators_setRelay(true);
    autoTriggered = true;
}

// Reset auto trigger when humidity becomes normal
if(data.humidity < HUMIDITY_WARN){
    autoTriggered = false;
}
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
 }
 




















