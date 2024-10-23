## xiao_i2c_write_register_value

This arduino example shows how to write the "Mute Speaker(0x10)" I2C register. User need to connect a speaker to the speaker connector(not the 3.5mm jack) on ReSpeaker Lite and flash an i2s firmware to XMOS XU316.


After uploading this arduino example to XIAO ESP32S3, users are able to hear that the speaker keep going on and off every second, that means the "Mute Speaker" command is working.