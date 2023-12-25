#ifndef SECRETS_H
#define SECRETS_H
/*
 * Secrets file detailing information needed to connect a specific sensor to the LoraWan network
 * Notes:
 Raw numbers can be found in application environment (Helium, TTN, etc..) when creating a device.
 All in big endian format (left to right), with 0x appended to each pair of values.
 DevEUI is device specific.
 AppEUI is application specific.
 AppKey is generated using both the device and app, making it app & device specific.
 */

/* V4-SENSOR-1*/

static uint8_t devEui[] = { 0x60, 0x81, 0xF9, 0xDB, 0xF1, 0xD0, 0x0C, 0xEA };
static uint8_t appEui[] = { 0x60, 0x81, 0xF9, 0x2E, 0x01, 0x72, 0x0F, 0x48 };
static uint8_t appKey[] = { 0x06, 0x38, 0xBB, 0x01, 0xA4, 0xEE, 0x7E, 0xD3, 0xDC, 0xF3, 0x1B, 0x82, 0xB2, 0xC1, 0x48, 0x39 };

#endif