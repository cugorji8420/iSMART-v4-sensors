#include "mcu_functions.h"
RTC_PCF8523 rtc;
#define SYNC_CLOCK true

//pin definitions
#define TRIGGER_PIN 13

Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);

/*
 * Description: Setup featherwing by initializing rtc if not yet initialized. 
 * Returns: void.
 */
void setup_featherwing() {
    //RTC init
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC...");
        Serial.flush();
        abort();
    }
    if(SYNC_CLOCK){
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

/*
 * Description: Clears the content in Serial2 by reading 64 bytes.
 * Returns: void.
 */ 
void clear_serial(){
    for(int i = 0; i < 64; i++){
        Serial2.read();
    }
}

/*
 * Description: Reads distance from ultrasonic sensor. 
 * Returns: uint16_t value of distance read from ultrasonic sensor. 
 */
uint16_t read_distance() {
    char buffer_RTT[4] = {0};
    uint16_t distance = 0;
    int attempts = 0;

    for(int attempts = 0; attempts < 5; attempts++){
        clear_serial();

        //TRIGGER_PIN needs to be pulled high for a minimum of 20 microseconds.
        digitalWrite(TRIGGER_PIN, HIGH);
        delayMicroseconds(25);            
        digitalWrite(TRIGGER_PIN, LOW);
        delay(500);

        if (Serial2.available() > 0) {
            do {
                if (Serial2.read() == 'R') {
                    for (int i = 0; i < 4; i++) {
                        buffer_RTT[i] = Serial2.read();
                    }
                }
            } while (buffer_RTT == NULL);
            distance = (buffer_RTT[0] - '0') * 1000 + (buffer_RTT[1] - '0') * 100 + (buffer_RTT[2] - '0') * 10 + (buffer_RTT[3] - '0');
            if(distance >= 300 && distance <= 5000){
                return distance;
            }
        }
        delay(100);
    }
    return 0;
}

String read_rain() {
    String inByte = "";
    Serial1.println("R");
    int curr_delay = 0;
    while(!Serial1.available() && curr_delay < 5){
        if(curr_delay != 0){
            Serial1.println("R");
        }
        delay(1000);
        curr_delay++;
    }
    if (Serial1.available()) {
        inByte = Serial1.readString();
    }
    return inByte;
}

String get_timestamp(void) {
    DateTime now = rtc.now();
    String custom_timestamp_calibrated = String(now.year(), DEC) + String('-') + String(now.month(), DEC) + String('-') + String(now.day(), DEC) + String(' ') + String(now.hour(), DEC) + String(':') + String(now.minute(), DEC) + String(':') + String(now.second(), DEC);
    return custom_timestamp_calibrated;
}

String readAll(senState* st) {
    //record time before sensor functions
    unsigned long curr_time = millis();

    //get distance reading from ultrasonic sensor
    uint16_t distance = read_distance();
    //get battery reading from analog pin
    float battery_voltage = get_battery_voltage();
    //get rain data from rain gauge
    String rain_msg = read_rain();

    //parse message from data obtained above
    String msg = parse_message(distance, rain_msg, get_timestamp(), String(battery_voltage));
    Serial.println("**Read Stable");
    //update passed in state for Lora packaging
    st->distance = distance;
    st->voltage = battery_voltage;
    st->acc = parse_rain(rain_msg);
    st->newData = true;
    Serial.println("**Write Stable");

    //print readings to serial port
    Serial.print(msg);

    //flush serial for good measure
    serial_flush();
    Serial.println("**Flush Stable");

    return msg;
}
