## xiao_i2c_get_register_value

This arduino example shows how to read the "VNR", "Mute Status" and "Firmware Version" I2C registers.

After uploading this arduino example to XIAO ESP32S3, users are able to 
- Speak up or keep voice down to find the VNR value goes up and down
- Press the mute button on the board to find the Mute Status toggled(and also the VNR value goes to 0 when mute active)
- Find the XMOS XU316 firmware version from the serial output too