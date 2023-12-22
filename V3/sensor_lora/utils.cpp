#include "utils.h"

#define VBATPIN A7

extern Uart Serial2;

/*
 * Description: Takes 16 bit number and splits into quotient and remainder using a key. 
 * Returns: int pointer to an array. 
 */
int* encode16(int num, int key){

  static int result[2];
  int quotient, remainder;
    
  if(num > (key*255)){
    remainder = 0;
    quotient = 255;
  }
  else{
    remainder = (num%key);
    quotient = (num - remainder)/key;
  }
    
  result[0] = quotient;
  result[1] = remainder;
  return result;
}

/*
 * Description: Prints out payload array entry by entry. 
 * Returns: void. 
 */
uint8_t* prep_payload(senState* data, uint8_t* arr, int key){
    float v, r;
    v = 100*(data->voltage);
    r = 1000*(data->acc);

    //encode relevant values

    int* volt = encode16((int)v, key);
    //voltage bytes (byte 0 reserved)
    arr[1] = volt[0];
    arr[2] = volt[1];

    int* dist = encode16((int)(data->distance), key);
    //distance bytes
    arr[3] = dist[0];
    arr[4] = dist[1];

    int* rain = encode16((int)r, key);
    //distance bytes
    arr[5] = rain[0];
    arr[6] = rain[1];

    //byte for padding
    arr[7] = 255;
    return arr;
}

/*
 * Description: Returns the battery level measured by the analog pin VBATPIN. 
 * Returns: String representation of battery level. i.e. "VBat = 3.4" 
 */
float get_battery_voltage(){
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat;

}

/*
 * Description: print unsigned value v in hexadecimal form. 
 * Returns: void 
 */
void print_as_hex(unsigned v) {
    v &= 0xff;

    if (v < 16){
        Serial.print('0');
    }

    Serial.print(v, HEX);
}

/*
 * Description: swaps uint16_t values p and q.
 * Returns: void. 
 */
void swap(uint16_t  *p, uint16_t  *q) {
    int t;

    t = *p;
    *p = *q;
    *q = t;
}

/*
 * Description: takes uint16_t distance, String rain_msg, String date_time, String bat_voltage and parses it into the desired reading format. 
 * Returns: String of parsed message. 
 */
String parse_message(uint16_t distance, String rain_msg, String date_time, String bat_voltage){
    String result = "";
    result = result + "Date/Time = " + date_time + ", Voltage: " + bat_voltage + "\n";
    result = result + "Distance " + String(distance) + "mm, " + rain_msg;
    return result;
}

/*
 * Description: Fluses messages in message queue for Serial1. 
 * Returns: void. 
 */
void serial_flush(void) {
    while (Serial2.available()) Serial2.read();
}

float parse_rain(String raw){
  char num[100];
  int i = 0;
  while (raw[i+3] != 'i' && i<10) {
    num[i] = raw[i+3];
    i++;
  }
  float temp = atof(num);
  return temp;
}
