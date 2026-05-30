#include "network.h"
#include "rpc.h"
static PubSubClient* mqtt =NULL;
static byte  mac[6]=NODE_MAC;
static unsigned long lastReconnectTime=0;

// This callback function routes incoming MQTT messages down to your RPC processing module
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Forward the packet straight to your rpc library handler
    rpc_mqttCallback(topic, payload, length); 
}

static void doConnect(void){

  //connect to things board
  if(mqtt->connect(CLIENT_ID,ACCESS_TOKEN,NULL))
  {
        Serial.println("MQTT connected");
         mqtt->subscribe(TOPIC_RPC_REQUEST);
  }
  else{
    // Serial.println("mqtt failed");
    // Serial.println(mqtt->state());
    Serial.print("MQTT failed, state = ");
    Serial.println(mqtt->state());

  }
}

void  network_begin(PubSubClient* mqttClient){
mqtt =mqttClient;

//connect board to the internet
// Ethernet.begin(mac);
// delay(2000);
if(Ethernet.begin(mac) == 0)
{
    Serial.println("DHCP FAILED");
}
else
{
    Serial.println("DHCP SUCCESS");

    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
}

delay(2000);    //for ethernet stabilization

//connect to things board
mqtt->setServer(TB_SERVER,TB_PORT);
doConnect();

}

void    network_maintain(void){
  Ethernet.maintain();

//if board connected to things board try to reconnect every 5 sec

if(mqtt->connected()){
  mqtt->loop();
  
}
else{
  unsigned  long now=millis();
  if(now -lastReconnectTime >=RECONNECT_INTERVAL){
    lastReconnectTime=now;
    doConnect();
    
  }

}
}

//return the status of mqtt connection
uint8_t network_isConnected(void){
  return (uint8_t)mqtt->connected();   //ret 1 if connected and 0-disconnected

}

uint8_t network_publish(const char* topic, const char* payload, uint8_t retain){
  return (uint8_t)mqtt->publish(topic,payload,(bool)retain);


}




