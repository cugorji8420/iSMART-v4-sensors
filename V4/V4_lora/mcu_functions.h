#include "utils.h"
/*
 * Function definitions for mcu_functions.cpp.
 */
float read_voltage(void);
String read_rain(senState* dt);
uint16_t read_distance(senState* dt);
int readAll(senState* st, bool print);

