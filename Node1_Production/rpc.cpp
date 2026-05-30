
//  rpc.cpp - parses raw MQTT JSON and dispatches RPC commands.
 
//   No ArduinoJson - uses strstr/strncmp directly on the payload buffer.
//   This saves ~500 B of flash compared to the ArduinoJson version.
 
//   ThingsBoard RPC format (stable):
//     {"method":"setRelay","params":true}
//     {"method":"setLED","params":{"pin":5,"state":true}}
//     {"method":"getStatus","params":null}
//     {"method":"reboot","params":null}
 

#include "rpc.h"
#include "actuator.h"
#include "SensorManager.h"
#include <avr/wdt.h>  //watchdog reset/reboot(used in avr microcontroller programming)


/* File-local state  set once by rpc_init() */
static PubSubClient* rpcMqtt  = NULL;  // stores mqtt client pointer
static char*         rpcBuf   = NULL;  // stores shared json buffer 
static uint8_t       rpcBufLen = 0;   //stores buffer size

/*  Internal helpers  */


/* Pull the request ID from the end of the topic string */
static void extractRequestId(const char* topic, char out[RPC_ID_SIZE]) {
    const char* slash = strrchr(topic, '/');     //finds the last '/' character
    if (slash) {
        strncpy(out, slash + 1, RPC_ID_SIZE - 1);  // copies the request id into out
    } else {
        out[0] = '\0';  //null termination to avoid corrupted strings
    }
    out[RPC_ID_SIZE - 1] = '\0';
}

/* Publish the response JSON to the matching response topic (To the things board )*/
static void sendResponse(const char* reqId, const char* json) {
    char topic[64];
    snprintf_P(topic, sizeof(topic), PSTR("%s%s"), TOPIC_RPC_RESPONSE, reqId);
    rpcMqtt->publish(topic, json);  
}

/* Command handlers  */

/* params is the raw string "true" or "false" */

static void handleSetRelay(const char* reqId, const char* params) {  //controls relay remotely
    uint8_t on = (params && strstr(params, "true")) ? 1 : 0;  //checking true / false
    actuators_setRelay(on);  //turns physical relay on / off
    snprintf_P(rpcBuf, rpcBufLen,
        PSTR("{\"success\":true,\"result\":\"%s\"}"),
        on ? "Relay ON" : "Relay OFF");
    sendResponse(reqId, rpcBuf);
}


/* Returns relay state, uptime, and free RAM */
static void handleGetStatus(const char* reqId) {     //sends the board status to the dashboard
    snprintf_P(rpcBuf, rpcBufLen,
        PSTR("{\"success\":true,\"relay\":%u,\"uptime\":%lu}"),
        (unsigned)actuators_getRelayState(),
        millis() / 1000UL);
    sendResponse(reqId, rpcBuf);
}



static void handleUnknown(const char* reqId) {  //unknown command handling
    snprintf_P(rpcBuf, rpcBufLen, PSTR("{\"success\":false,\"code\":404}"));
    sendResponse(reqId, rpcBuf);
}

/*  Public functions  */

void rpc_init(PubSubClient* mqtt, char* buf, uint8_t bufLen) {
    rpcMqtt   = mqtt;
    rpcBuf    = buf;
    rpcBufLen = bufLen;
}

/*
 * MQTT callback — called by PubSubClient when a message arrives.
 * Parses method name with strstr (no JSON library needed),
 * then calls the matching handler.
 */
void rpc_mqttCallback(char* topic, byte* payload, unsigned int length) {
    const char* mStart;
    const char* params;
    char reqId[RPC_ID_SIZE];

    if (!rpcMqtt) return;

    /* Copy payload into our buffer and null-terminate it */
    if (length >= rpcBufLen) length = rpcBufLen - 1;
    memcpy(rpcBuf, payload, length);       //Copies raw MQTT bytes into string buffer.
    rpcBuf[length] = '\0';

    extractRequestId(topic, reqId);

    /* Locate "method":"<name>" in the raw JSON string */
    mStart = strstr(rpcBuf, "\"method\":\"");
    if (!mStart) { handleUnknown(reqId); return; }
    mStart += 10;   /* skip past:  "method":" */

    if (!strchr(mStart, '"')) { handleUnknown(reqId); return; }

    /* Locate "params": value (pointer to whatever follows the colon) */
    params = strstr(rpcBuf, "\"params\":");
    if (params) params += 9;   /* skip past:  "params": */

    /* Dispatch based on the method name */
    if      (strncmp_P(mStart, PSTR("setRelay"),  8) == 0) handleSetRelay (reqId, params);
    else if (strncmp_P(mStart, PSTR("getStatus"), 9) == 0) handleGetStatus(reqId);
    else                                                    handleUnknown  (reqId);
}
