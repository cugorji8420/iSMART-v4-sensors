#include <Arduino.h>
#include "softSerial.h"

#ifndef UTILS_H
#define UTILS_H

/*
 * Description: Struct to capture and store code-level configurations and network settings.  
 * Usage: Used to maintain global information about connection status, sensor configurations, and lora behaviour.
 */
typedef struct {
    int conn;
    int attempts;
    int freq;
    int fsleep;
    float ref;
} confState;

/*
 * Description: Struct to capture and store code-level data about various readings and status.  
 * Usage: Used to pass global information about sensor readings and battery voltage, as well as any error codes.
 */
typedef struct {
    int distance;
    float rain[4];
    float voltage;
    int err;
    confState* cfig;
} senState;

/*
 * Function definitions for utils.cpp.
 */
uint8_t* prep_payload(senState* data, uint8_t* arr, int key, bool printout = false);
int* encode16(int num, int key);
void print_as_hex(unsigned v);
void swap(uint16_t *p, uint16_t *q);
void printSenstate(senState* data);
String parse_message(uint16_t distance, String rain_msg, String bat_voltage);
void parse_rain(String raw, senState* st);
softSerial mySERCOM(uint8_t tx_GPIO, uint8_t rx_GPIO, int sr_BAUD);
void clear_serial(softSerial ser_COM);
#endif