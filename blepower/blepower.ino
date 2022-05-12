
// FQBN esp32:esp32:featheresp32

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
}

void loop() {
  // A13 is half battery voltage
  int analogValue = analogRead(A13);
  int batteryVolts = 2*analogReadMilliVolts(A13);

  // print out the values you read:
  Serial.printf("ADC battery mVolts = %dmV [%d]\n",batteryVolts, analogValue);

  delay(200);  // delay in between reads for clear read from serial
}
