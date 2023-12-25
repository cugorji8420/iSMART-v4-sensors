#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"
#include "utils.h"
#include "mcu_functions.h"
#include "secrets.h"

/*
 * Packet size definition.
 */
#define DATA_LEN 14

/*
 * Function definitions for lora.cpp.
 */
static void wakeUp();
static void lowPowerSleep(uint32_t sleeptime);
void lora_setup();
int join_loop(int fsleep);
void lora_loop(confState* conf, senState* data);
void downLinkDataHandle(McpsIndication_t *mcpsIndication);



