#include "V4_lora.h"
senState sensors;
confState conf;

void setup(){
	//serial init
  Serial.begin(115200);

  conf.conn = 0;
  conf.attempts = 6;
  conf.freq = 40000;
  conf.fsleep = 30000;
  conf.ref = 11;

  sensors.cfig = &conf;
  Serial.println("----------------Booting Up----------------");
  lora_setup();
}

void loop(){
  Serial.println("----------------Read Start----------------");
  readAll(&sensors, true);
  Serial.println("----------------Read Finish----------------");
  Serial.println("**********LoraWAN Start********** ");
  lora_loop(&conf, &sensors);
  delay(conf.freq);
}
