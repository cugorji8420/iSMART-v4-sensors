#include "lora.h"

/*
 * Definitions for loraWan based on the dev-board's flashed RGB
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

///Channel Mask 7 [DO NOT CHANGE WITHOUT NETWORK KNOWLEDGE]
uint16_t userChannelsMask[6]= { 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };
static uint8_t counter=0;

///////////////////////////////////////////////////

/*
 * Description: Utilities for going into low power mode
 * All Functions Return: void.
 */

TimerEvent_t sleepTimer;
//Records whether our sleep/low power timer expired
bool sleepTimerExpired;
//Function for board wake
static void wakeUp()
{
  sleepTimerExpired=true;
}
//Function for board sleep
static void lowPowerSleep(uint32_t sleeptime)
{
  sleepTimerExpired=false;
  TimerInit( &sleepTimer, &wakeUp );
  TimerSetValue( &sleepTimer, sleeptime );
  TimerStart( &sleepTimer );
  //Low power handler also gets interrupted by other timers
  //So wait until our timer had expired
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop( &sleepTimer );
}
///////////////////////////////////////////////////
/*
 * Description: Sets up lora transmission on region frequency.
 * Returns: void.
 */
void lora_setup() {
  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);
  //Enable DR 1
  LoRaWAN.setFixedDR(1);
}

/*
 * Description: Takes in sleep seconds. Attempts to join the network. Sleeps board for sleep seconds if join fails.
 * Returns: int connection status.
 */
int join_loop(int fsleep){
  while (1) {
    Serial.println("Joining...");
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      Serial.println("JOIN FAILED! Sleeping for " + String(fsleep/1000) + " seconds...");
      lowPowerSleep(fsleep);
    } else {
      Serial.println("JOINED");
      LoRaWAN.setFixedDR(1);
      return 1;
    }
  }
}
////////////////////////////////////////////////////
/*
 * Description: Main lora loop. Handles connection status and data sending.
 * Returns: void.
 */
void lora_loop(confState* conf, senState* data){
  uint8_t* payload = (uint8_t*)malloc(sizeof(uint8_t) * DATA_LEN);
  switch(conf->conn){
    case -1:
      Serial.println("Connection error...resetting");
      conf->conn = 0;
      break;

    case 0:
      conf->conn = join_loop(conf->fsleep);
      break;

    case 1:
      counter++;
      delay(10);
      prep_payload(data, payload, 256);  
      //Now send the data. The parameters are "data size, data pointer, port, request ack"
      Serial.printf("\nSending.... Packet Counter=%d\n", counter);
      //Here we send confirmed packed (ACK requested) only for the first five (remember there is a fair use policy)
      bool requestack=counter<5?true:false;
      if (LoRaWAN.send(DATA_LEN, payload, 1, requestack)) {
        Serial.println("Send OK");
        delay(100);
      } else {
      Serial.println("Send FAILED");
    }
  }
  free(payload);
}
///////////////////////////////////////////////////
/*
 * Description: Handles any downlinks from the network. 
 * Returns: void.
 */
void downLinkDataHandle(McpsIndication_t *mcpsIndication){
  Serial.printf("Received downlink: %s, RXSIZE %d, PORT %d, DATA: ", mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
}
