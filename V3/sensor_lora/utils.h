#include <Arduino.h>
#include "src/SdFat2/SdFat.h"
#include "src/RTCLib2/RTClib.h"
#include <fstream>

#ifndef UTILS_H
#define UTILS_H
typedef struct {
    int distance;
    float voltage;
    float acc;
    bool newData;
} senState;

uint8_t* prep_payload(senState* data, uint8_t* arr, int key);
int* encode16(int num, int key);
float get_battery_voltage(void);
void print_as_hex(unsigned v);
void swap(uint16_t *p, uint16_t *q);
String parse_message(uint16_t distance, String rain_msg, String date_time, String bat_voltage);
void serial_flush(void);
float parse_rain(String raw);
#endif