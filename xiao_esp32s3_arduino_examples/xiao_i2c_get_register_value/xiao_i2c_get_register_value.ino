#include "Wire.h"

#define XMOS_ADDR     0x42

uint8_t xmos_read_1byte(uint8_t resid, uint8_t cmd, uint8_t *value)
{
    return xmos_read_bytes(resid, cmd, value, 1);
}

uint8_t xmos_read_bytes(uint8_t resid, uint8_t cmd, uint8_t *value, uint8_t read_byte_num)
{
    uint8_t read_result;
    if (read_byte_num == 0 || read_byte_num > 254) return 0xFF;
    read_byte_num++;
    Wire.beginTransmission(XMOS_ADDR);
    Wire.write(resid);
    Wire.write(cmd);
    Wire.write(read_byte_num); // read_byte_num
    Wire.endTransmission();

    Wire.requestFrom(XMOS_ADDR, read_byte_num);
    
    read_result = Wire.read();
    for (uint8_t i = 0; i < read_byte_num - 1; i++) {
        value[i] = Wire.read();
    }
    
    return read_result;
}

void setup(void) {  
  // Open Serial 
  Serial.begin(115200);
  Wire.begin(5,6);
  // while(!Serial);

  Serial.println("started...");
}

uint8_t vnr_value = 0;
uint8_t mute_status = 0;
uint8_t firmware_version[3] = {0};
uint32_t now_time = 0;

void loop() {
    if (millis() - now_time > 1000) {
        now_time = millis();

        // Get VNR value(need i2s firmware >= v1.0.5)
        xmos_read_1byte(0xF1, 0x80, &vnr_value);
        Serial.print("VNR value(0-100): ");
        Serial.println(vnr_value);

        // Get Mute Status(need i2s firmware >= v1.0.9), 1: mute, 0: not mute
        xmos_read_1byte(0xF1, 0x81, &mute_status);
        if (mute_status) Serial.println("Mute active!");
        else Serial.println("Mute inactive...");

        // Get Firmware Version
        xmos_read_bytes(0xF0, 0xD8, firmware_version, 3);
        Serial.print("Firmware Version: v");
        Serial.print(firmware_version[0]);
        Serial.print(".");
        Serial.print(firmware_version[1]);
        Serial.print(".");
        Serial.println(firmware_version[2]);
    }
}