#include "utils.h"

/*
 * Description: Takes 16 bit number and splits into quotient and remainder using a key.
 * Usage: Used during payload creation to split larger data values into component values.
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
 * Description: Takes in global sensor data, a pointer to payload array, and a key. Fills payload array with encoded sensor data.
 * Usage: Used during payload creation to split larger data values into component values.
 * Returns: int pointer to payload array. 
 */
uint8_t* prep_payload(senState* data, uint8_t* arr, int key, bool printout){
    printSenstate(data);
    int i = 1;
    float v, r;
    v = 100 * (data->voltage);
    
    //error bits
    arr[0] = data->err;

    Serial.println("");
    int* volt = encode16((int)v, key);
    //voltage bits
    arr[i] = volt[0];
    arr[i+1] = volt[1];
    i += 2;
  
    int* dist = encode16((int)(data->distance), key);
    //distance bits
    arr[i] = dist[0];
    arr[i+1] = dist[1];
    i += 2;

    int rainLen = sizeof(data->rain) / sizeof(data->rain[0]);
    //rain bits [acc, ev_acc, tot_acc, rph]
    for(int j = 0; j < rainLen; j++){
      r = 1000*(data->rain[j]);
      int* r2 = encode16((int)r, key);
      arr[i] = r2[0];
      arr[i+1] = r2[1];
      i += 2;
    }

    //bit for padding
    arr[i] = 255;
    
    if(printout == true){
    //payload print out
    Serial.println("Payload");
    for(i = 0; i < 14; i++){
      Serial.println("[" + String(i) + "]" + String(arr[i]) + " ");
    }
    Serial.println("");
    }
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
 * Description: Takes uint16_t distance, String rain_msg, String date_time, String bat_voltage and parses into the desired reading format.
 * Usage: Used during sensor readAll to format print out of sensor read values whenever an error occurs.
 * Returns: String of parsed message.
 */
String parse_message(uint16_t distance, String rain_msg, String bat_voltage){
    String result = "";
    result = result + "Voltage: " + bat_voltage + "\n";
    result = result + "Distance: " + String(distance) + "mm\n" + "Rain: " + rain_msg;
    return result;
}

/*
 * Description: Takes in global sensor data and prints out stored data.
 * Usage: Used for debugging value assignment for sensors.
 * Returns: void.
 */
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

/*
 * Description: Extracts numerical values from raw String containing numbers and puts them into global sensor data.
 * Usage: Used during sensor readAll to parse out rain gauge String response.
 * Returns: void.
 */
void parse_rain(String raw, senState* st){
  String strs[20];
  int StringCount = 0;
  // Split the string into substrings
  while (raw.length() > 0){
    int index = raw.indexOf(' ');
    if (index == -1){
      strs[StringCount++] = raw;
      break;
    }
    else{
      strs[StringCount++] = raw.substring(0, index);
      raw = raw.substring(index+1);
    }
  }
  // Create rain gauge array
  int j = 0;
  for (int i = 0; i < StringCount; i++){
    const char* str = strs[i].c_str();
    if(str[0] >= '0' && str[0] <= '9' ){
      st->rain[j] = atof(str);
      j++;
    }
  }
}

/*
 * Description: Takes in desired TX pin, RX pin, and baudrate, to create and begin a software serial instance.
 * Usage: Used to create serial port for each sensor before sensor read happens.
 * Returns: void.
 * Notes:
 Only one software serial can run at a time. Most recently started software serial will run and invalidate any other serial.
 Reinitializing an older serial without first closing the most recent serial will also invalidate the reinitialized serial.
 */
softSerial mySERCOM(uint8_t tx_GPIO, uint8_t rx_GPIO, int sr_BAUD)
{
  softSerial SerialCOM(tx_GPIO /*TX pin*/, rx_GPIO /*RX pin*/);
  SerialCOM.begin(sr_BAUD);
  return SerialCOM;
}

/*
 * Description: Takes in software serial port and clears its buffer.
 * Usage: Used after distance sensor read to clear buffer.
 * Returns: void.
 */
void clear_serial(softSerial ser_COM){
  ser_COM.flush();
  delay(100);
}

