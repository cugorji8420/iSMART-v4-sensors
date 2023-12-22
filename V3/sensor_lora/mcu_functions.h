#include <Arduino.h>
#include "src/SdFat2/SdFat.h"
#include "src/RTCLib2/RTClib.h"
#include "utils.h"
#include <fstream>

String readAll(senState* st);
void setup_featherwing(void);
uint16_t read_distance(void);
uint16_t read_distance_using_modes(unsigned int sensor_mode, unsigned int sensor_sampling_rate, unsigned sample_size);
String read_rain(void);
String get_timestamp(void);
void delay_period(unsigned long curr_time, unsigned long period);
String read_distance_raw(void);
