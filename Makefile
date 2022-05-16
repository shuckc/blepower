
# arduino-cli lib install "Adafruit GFX Library"
# arduino-cli lib install "Adafruit SSD1306"

arduino-cli compile --fqbn esp32:esp32:featheresp32 ~/repos/blepower/blepower -v
arduino-cli upload -p /dev/ttyUSB3 --fqbn esp32:esp32:featheresp32 blepower
arduino-cli monitor -p /dev/ttyUSB3 -c baudrate=115200


arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:featheresp32 blepower
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
