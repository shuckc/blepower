
arduino-cli compile --fqbn esp32:esp32:featheresp32 ~/repos/blepower/blepower -v
arduino-cli upload -p /dev/ttyUSB3 --fqbn esp32:esp32:featheresp32 blepower
arduino-cli monitor -p /dev/ttyUSB3 -c baudrate=115200

