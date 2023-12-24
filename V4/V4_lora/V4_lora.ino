#include "V4_lora.h"
senState sensors;
confState conf;
#ifdef __asr650x__
#include "innerWdt.h"
#endif

///Definitions needed for running
static TimerEvent_t wakeUpBoard;
uint8_t lowpower=0;
#ifdef __asr650x__
#define MAX_FEEDTIME 2800// ms
#else
#define MAX_FEEDTIME 24000// ms
#endif
bool autoFeed = true;
///--------------------------------

void onSleep(int timetillwakeup)
{
  Serial.printf("Going into lowpower mode. Will wake in %d seconds.\r\n",(timetillwakeup/1000));
  Serial.println("...");
  lowpower=1;
  //timetillwakeup ms later wake up;
  TimerSetValue( &wakeUpBoard, timetillwakeup );
  TimerStart( &wakeUpBoard );
}
void onWakeUp()
{
  Serial.println("Woke up!");
  lowpower=0;
}

void setup(){
	//serial init
  Serial.begin(115200);
  innerWdtEnable(autoFeed);

  conf.conn = 0;
  conf.attempts = 6;
  conf.freq = 55000;
  conf.fsleep = 30000;
  conf.ref = 11;

  sensors.cfig = &conf;
  TimerInit( &wakeUpBoard, onWakeUp );
  Serial.println("----------------Booting Up----------------");
  lora_setup();
}

void loop(){
  if(lowpower){
    lowPowerHandler();
  }
  else{
    Serial.println("----------------Read Start----------------");
    readAll(&sensors, true);
    Serial.println("----------------Read Finish----------------");
    Serial.println("**********LoraWAN Start********** ");
    lora_loop(&conf, &sensors);
    delay(100);
    onSleep(conf.freq);
  }
}
