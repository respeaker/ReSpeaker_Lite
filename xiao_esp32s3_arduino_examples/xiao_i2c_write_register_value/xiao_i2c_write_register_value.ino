#include "AudioTools.h"
#include "Wire.h"

#define XMOS_ADDR     0x42

AudioInfo info_16k(16000, 2, 32);
SineWaveGenerator<int16_t> sineWave(1000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);            // Stream generated from sine wave

I2SStream out; 
I2SConfig config_i2s1;

StreamCopy copier(out, sound);

void xmos_write_1byte(uint8_t resid, uint8_t cmd, uint8_t value)
{
    xmos_write_bytes(resid, cmd, &value, 1);
}

void xmos_write_bytes(uint8_t resid, uint8_t cmd, uint8_t *value, uint8_t write_byte_num)
{
    Wire.beginTransmission(XMOS_ADDR);
    Wire.write(resid);
    Wire.write(cmd);
    Wire.write(write_byte_num); // write_byte_num
    for (uint8_t i = 0; i < write_byte_num; i++) {
        Wire.write(value[i]);
    }
    Wire.endTransmission();
}

void setup(void) {  
    // Open Serial 
    Serial.begin(115200);
    Wire.begin(5,6);
    // while(!Serial);

    // Start I2S
    Serial.println("starting I2S...");
    config_i2s1 = out.defaultConfig(TX_MODE);
    config_i2s1.copyFrom(info_16k); 
    // Custom I2S output pins
    config_i2s1.pin_bck = 8;
    config_i2s1.pin_ws = 7;
    config_i2s1.pin_data = 43;
    config_i2s1.pin_data_rx = 44;
    config_i2s1.is_master = false;
    out.begin(config_i2s1);

    // Generate a sine wave sound and play it through speaker
    sineWave.begin(info_16k, N_B4);

    Serial.println("started...");
}

uint32_t now_time = 0;
uint8_t mute_status = 0;

void loop() {
    copier.copy();
    if (millis() - now_time > 1000) {
        now_time = millis();
        // Toggle speaker mute status, you can hear that the speaker keep going on and off every second
        xmos_write_1byte(0xF1, 0x10, mute_status);
        if (!mute_status) Serial.println("speaker is muted");
        else Serial.println("speaker is enabled");
        mute_status = 1 - mute_status;
    }
}