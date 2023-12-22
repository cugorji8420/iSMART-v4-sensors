#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"
#include "utils.h"
#include "mcu_functions.h"
#include "secrets.h"

#define DATA_LEN 14

static void wakeUp();
static void lowPowerSleep(uint32_t sleeptime);
void lora_setup();
int join_loop(int fsleep);
void lora_loop(confState* conf, senState* data);
void downLinkDataHandle(McpsIndication_t *mcpsIndication);



