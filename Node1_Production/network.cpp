#include "network.h"
static PubSubClient* mqtt =NULL;// Pubsubclient(contains func like connect(),publish(),subscribe(),loop())is an mqtt library class
static byte  mac[6]=NODE_MAC;      //  
static unsigned long lastReconnectTime=0;

//the static functions are only usable (inside this file)

static void doConnect(void){    //internal mqtt connection function
  //connecting arduino  to things board

  if(mqtt->connect(CLIENT_ID,ACCESS_TOKEN,NULL))   //MQTT Login request
  {
        Serial.println("MQTT connected");
        mqtt->subscribe(TOPIC_RPC_REQUEST);
  }
  else{
  
    Serial.print("MQTT failed, state = ");
Serial.println(mqtt->state());  // prints the mqtt states ie 0-connected
                                                     //     -1-disconnected
                                                     //     -2-connect failed
                                                    //      -4-timeout
  }
}

void  network_begin(PubSubClient* mqttClient){
mqtt =mqttClient;


if(Ethernet.begin(mac) == 0)                // DHCP CONNECTION
{ 
    Serial.println("DHCP FAILED");
}
else
{
    Serial.println("DHCP SUCCESS");      // it means router alocated an ip to the arduino 

    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
}

delay(2000);

//connect to things board
mqtt->setServer(TB_SERVER,TB_PORT);   //defines mqtt broker address
doConnect();                         // TB_SERVER AND TB_PORT defined in config.h

}



void network_maintain(void){
  Ethernet.maintain();    //keeps ethernet alive(dhcp may get expire sometime,
                                               // helps to renew IP address)

if(mqtt->connected()){          //checks if mqtt is connected
  mqtt->loop();               //continuosly checks if mqtt is alive(also helps in rpc reception )          
  
}
else{
  unsigned  long now=millis();
  if(now - lastReconnectTime >=RECONNECT_INTERVAL){
    lastReconnectTime=now;
    doConnect();
  }

}
}

//return the status of mqtt connection
uint8_t network_isConnected(void){
  return (uint8_t)mqtt->connected();

}


//sending the mqtt messages to the things board
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain){
  return (uint8_t)mqtt->publish(topic,payload,(bool)retain);
}

//topic-->mqtt destination
//payload-->json file to be passed to things board
//retain-->broker retain flag




