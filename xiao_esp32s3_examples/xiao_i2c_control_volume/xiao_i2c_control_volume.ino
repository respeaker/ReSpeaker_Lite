
#include "AudioTools.h"
#include "Wire.h"

#define AIC3204_ADDR  0x18

AudioInfo info_16k(16000, 2, 32);
SineWaveGenerator<int16_t> sineWave(1000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave

I2SStream out; 
I2SConfig config_i2s1;

StreamCopy copier(out, sound);

uint32_t now_time = 0;
uint8_t level_index = 0;
int8_t level[10] = {0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x00, 0x01, 0x02, 0x03};

void aic3204_write_reg(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(AIC3204_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Arduino Setup
void setup(void) {  
    // Open Serial 
    Serial.begin(115200);
    Wire.begin(5,6);
    // while(!Serial);

    // start I2S
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

    //  converter.begin(info_16k, info_48k);
    sineWave.begin(info_16k, N_B4);

    Serial.println("started...");
    
}

// Arduino loop - copy sound to out 
void loop() {
   copier.copy();
    if (millis() - now_time > 1000) {
        now_time = millis();
        // Select Page 1
        aic3204_write_reg(0x00, 0x01);
        // headphone left driver gain
        aic3204_write_reg(0x10, level[level_index]);
        // headphone right driver gain
        aic3204_write_reg(0x11, level[level_index]);
        // lineout left driver gain
        aic3204_write_reg(0x12, level[level_index]);
        // lineout right driver gain
        aic3204_write_reg(0x13, level[level_index]);
        Serial.print("level: ");
        Serial.println(level_index);
        
        level_index = level_index + 2;
        if (level_index >= 10) level_index = 0;
    }
}