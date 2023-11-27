#include "utils.h"

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
    printSenstate(data);
    int i = 1;
    float v, r;
    v = 100 * (data->voltage);
    
    //error byte
    arr[0] = data->err;

    Serial.println("");
    int* volt = encode16((int)v, key);
    //voltage bytes
    arr[i] = volt[0];
    arr[i+1] = volt[1];
    i += 2;
  
    int* dist = encode16((int)(data->distance), key);
    //distance bytes
    arr[i] = dist[0];
    arr[i+1] = dist[1];
    i += 2;

    int rainLen = sizeof(data->rain) / sizeof(data->rain[0]);
    //rain bytes
    for(int j = 0; j < rainLen; j++){
      r = 1000*(data->rain[j]);
      int* r2 = encode16((int)r, key);
      arr[i] = r2[0];
      arr[i+1] = r2[1];
      i += 2;
    }

    //byte for padding
    arr[i] = 255;
      
    Serial.println("Payload");
    for(i = 0; i < 14; i++){
      Serial.println("[" + String(i) + "]" + String(arr[i]) + " ");
    }
    Serial.println("");
    return arr;
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
String parse_message(uint16_t distance, String rain_msg, String bat_voltage){
    String result = "";
    result = result + "Voltage: " + bat_voltage + "\n";
    result = result + "Distance: " + String(distance) + "mm\n" + "Rain: " + rain_msg;
    return result;
}

void printSenstate(senState* data){
    Serial.println("");
    Serial.println("Err: "+ String(data->err));
    Serial.println("S-Dist: "+ String(data->distance));
    Serial.print("S-Rain: ");
    for(int i = 0; i < 4; i++){
      Serial.print(String(data->rain[i]) + " ");
    }
    Serial.println("");
    Serial.println("S-Volt: "+ String(data->voltage));
}

void parse_rain(String raw, senState* st){
  String strs[20];
  int StringCount = 0;
  // Split the string into substrings
  while (raw.length() > 0){
    int index = raw.indexOf(',');
    if (index == -1){
      strs[StringCount++] = raw;
      break;
    }
    else{
      strs[StringCount++] = raw.substring(0, index);
      raw = raw.substring(index+1);
    }
  }

  //Creating rain gauge array
  int j = 0;
  for (int i = 0; i < StringCount; i++){
    const char* str = strs[i].c_str();
    if(str[0] > 47 && str[0] < 58){
      st->rain[j] = atof(str);
      j++;
    }
  }
}

softSerial mySERCOM(uint8_t tx_GPIO, uint8_t rx_GPIO, int sr_BAUD)
{
  softSerial SerialCOM(tx_GPIO /*TX pin*/, rx_GPIO /*RX pin*/);
  SerialCOM.begin(sr_BAUD);
  return SerialCOM;
}

void clear_serial(softSerial ser_COM){
  ser_COM.flush();
  delay(100);
}

