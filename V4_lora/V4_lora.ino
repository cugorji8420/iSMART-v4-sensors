#include "V4_lora.h"
senState sensors;
confState conf;

void setup(){
	//serial init
  Serial.begin(115200);

  //Set rain accumulator to polling mode
  softSerial Serial3 = mySERCOM(GPIO4, GPIO5, 9600);
  Serial3.write("P");
  Serial3.end();
  

  conf.conn = 0;
  conf.attempts = 6;
  conf.freq = 20000;
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
  Serial.println("**********LoraWAN Start**********");
  lora_loop(&conf, &sensors);
  delay(1000);
}
