#include "mcu_functions.h"

float read_voltage(confState* conf) {
  ///pinMode(VBAT_ADC_CTL, OUTPUT);
  //digitalWrite(VBAT_ADC_CTL, LOW);
  uint16_t mV = analogRead(ADC)*2;
  ///pinMode(VBAT_ADC_CTL, INPUT);
  float volts = mV; 
  volts = (volts * conf->ref) / 1000;
  return volts;
}

String read_rain(senState* dt) {
    softSerial Serial3 = mySERCOM(GPIO4, GPIO5, 9600);
    String inByte = "-1";
    int attempts = 0;
    //Serial.println(Serial1.println("R"));
    int curr_delay = 0;
    while(!Serial3.available() && curr_delay < 15){
        if(curr_delay != 0){
          Serial3.println("R");
        }
        delay(1000);
        curr_delay++;
    }
    if (Serial3.available()) {
      Serial.println("Reading Rain");
      while (Serial3.peek() == -1){
        if(attempts > dt->cfig->attempts){
          inByte ="ERR";
          dt->err += 20;
          Serial3.end();
          return inByte;
        }
        attempts++;
      }
      inByte = Serial3.readString();
      Serial3.flush();
      Serial3.end();
      return inByte;
    }
    inByte ="NULL";
    dt->err += 10;
    Serial3.flush();
    Serial3.end();
    return inByte;
}

uint16_t read_distance(senState* dt) {
    char buffer_RTT[4] = {0};
    uint16_t distance = 0;
    int attempts = 0;
    dt->distance = distance;
    //RE-INIT Serial2 for Distance
    softSerial Serial2 = mySERCOM(GPIO1, GPIO2, 9600);
    uint8_t TRIGGER_PIN = GPIO3;
    while(!Serial2.available()){
      if (attempts > 100){
        dt->err += 1;
        Serial2.end();
        return -2;
      }
      attempts += 1;
      delay(20);
    }
    if (Serial2.available()) {
    Serial.println("Reading Distance");
    for(attempts = 0; attempts < dt->cfig->attempts; attempts++){
        delay(100);
        clear_serial(Serial2);
        //TRIGGER_PIN needs to be pulled high for a minimum of 20 microseconds.
        digitalWrite(TRIGGER_PIN, HIGH);
        delayMicroseconds((attempts+1) * 22);            
        digitalWrite(TRIGGER_PIN, LOW);
        delay(100);

        if (Serial2.available() > 0) {
          Serial.print(".." + String(attempts+1) +  ".. ");
          do{
              if (Serial2.read() == 'R') {
                for (int i = 0; i < 4; i++) {
                  buffer_RTT[i] = Serial2.read();
                }
              }
            } while (buffer_RTT == NULL);
          distance = (buffer_RTT[0] - '0') * 1000 + (buffer_RTT[1] - '0') * 100 + (buffer_RTT[2] - '0') * 10 + (buffer_RTT[3] - '0');
          if(distance >= 300 && distance < 5000){
            Serial2.flush();
            dt->distance = distance;
            Serial2.end();
            return distance;
          }
        }
        delay(100);
    }
    dt->err += 2;
    Serial2.end();
    return -1;
  }
}

int readAll(senState* st, bool print) {
    st->err = 0;
    bool err = false;
    //record time before sensor functions
    unsigned long curr_time = millis();
    //get distance reading from ultrasonic sensor
    String rain_msg = read_rain(st);
    delay(100);
    uint16_t distance = read_distance(st);
    //get battery reading from analog pin
    float battery_voltage = read_voltage(st->cfig);
    //get rain data from rain gauge
    Serial.println();
    //parse message from data obtained above
    String msg = parse_message(distance, rain_msg, String(battery_voltage));
    //update passed in state for Lora packaging
    st->voltage = battery_voltage;

    if(distance < 300 || distance > 5000){
      st->distance = 0;
      err = true;
    }
    else{
      st->distance = distance;
    }

    if(rain_msg == "ERR" || rain_msg == "NULL"){
      st->rain[0] = 0.0;
      st->rain[1] = 0.0;
      st->rain[2] = 0.0;
      st->rain[3] = 0.0;
      err = true;
    }
    else{
      parse_rain(rain_msg, st);
    }
    
    if(err){
      Serial.println("\nRead ERR...");
      Serial.println(msg);
      delay(200);
      return -1;
    }
    else{
      if(print){
        Serial.println(msg);
      }
    }
    Serial.println("\nRead OK");
    delay(200);
    return 0;
}
