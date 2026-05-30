
//Network
#define NODE_MAC    { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }

//thingsboard
#define TB_SERVER      "mqtt.thingsboard.cloud"
#define TB_PORT        1883
#define ACCESS_TOKEN   "Your_real_token"
#define CLIENT_ID      "WearHouseNode02"

//MQTT topic
#define TOPIC_TELEMETRY   "v1/devices/me/telemetry"
#define TOPIC_RPC_REQUEST   "v1/devices/me/rpc/request/+"  // board listens commands from dashboard
#define TOPIC_RPC_RESPONSE  "v1/devices/me/rpc/response/"  //reply from board 

//time delay
#define TELEMETRY_INTERVAL  5000UL
#define RECONNECT_INTERVAL  5000UL
#define MOTION_CLEAR_MS     10000UL  

// #define LDR_DARK_THRESH     300 


//pins used

//  Pins  (D10–D13 reserved for W5100 SPI) 
#define PIN_DHT         2
#define PIN_PIR         3
#define PIN_RELAY       4
#define PIN_LED_GREEN   5
#define PIN_LED_YELLOW  6
#define PIN_LED_RED     8
#define PIN_DOOR        7    // Magnetic door sensor (LOW = open)
#define PIN_LDR         A0
// A1–A5 available

//  Sensor config 
#define DHT_TYPE         DHT22
#define LDR_SAMPLES      4   //take 4 readings and avg them
#define LDR_SAMPLE_DLY   2   // delay between the reading is 2ms
#define LDR_DARK_THRESH  300   // ADC value below = dark (lights needed) // defining the threhold for isdark ie ldrval < 300[dark]
#define MIN_FREE_SRAM    256    //safety threshold

//  Thresholds for LED status 
#define TEMP_WARN        28.0f   // Warehouse: for warning >28
#define TEMP_CRIT        35.0f    //critical >35
#define HUMIDITY_WARN    70.0f   // High humidity = condensation risk
#define HUMIDITY_CRIT    85.0f
#define RPC_ID_SIZE      12    //request id string




