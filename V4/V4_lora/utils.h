#include <Arduino.h>
#include "softSerial.h"

#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int conn;
    int attempts;
    int freq;
    int fsleep;
    float ref;
} confState;

typedef struct {
    int distance;
    float rain[4];
    float voltage;
    int err;
    confState* cfig;
} senState;

uint8_t* prep_payload(senState* data, uint8_t* arr, int key);
int* encode16(int num, int key);
void print_as_hex(unsigned v);
void swap(uint16_t *p, uint16_t *q);
void printSenstate(senState* data);
String parse_message(uint16_t distance, String rain_msg, String bat_voltage);
void parse_rain(String raw, senState* st);
softSerial mySERCOM(uint8_t tx_GPIO, uint8_t rx_GPIO, int sr_BAUD);
void clear_serial(softSerial ser_COM);
#endif