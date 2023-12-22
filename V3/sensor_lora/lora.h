#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "secrets.h"
#include "mcu_functions.h"
#include "utils.h"
#ifndef LORA_H
#define LORA_H
void setup_lora(void);
void loop_lora(void);
#endif