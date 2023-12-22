#include "sensor_lora.h"

extern Uart Serial2;

void SERCOM1_Handler()
{
    Serial2.IrqHandler();
}

void setup() {
    delay(1000);
    
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);

    setup_featherwing();    
    pinPeripheral(11, PIO_SERCOM);

    setup_lora();
}

void loop() {
    loop_lora();
}
