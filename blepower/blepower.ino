//
// Board FQBN esp32:esp32:featheresp32
// dependancies
//  * Adafruit_SSD1306
//  * RTClib https://adafruit.github.io/RTClib/html/index.html
//  * ADS1115_WE https://github.com/wollewald/ADS1115_WE
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include<ADS1115_WE.h>  // https://github.com/wollewald/ADS1115_WE
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_ADDRESS_ADS1115 0x48
#define I2C_ADDRESS_SSD1306 0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_Millis rtc;

ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS_ADS1115);

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.printf("Screen init\n");
  if(!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS_SSD1306)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Serial.printf("RTC init\n");
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  // if (! rtc.begin()) {
  //  Serial.println("Couldn't find RTC");
  //  Serial.flush();
  //}

  //if (! rtc.isrunning()) {
  //  Serial.println("RTC is NOT running, let's set the time!");
  //  // When time needs to be set on a new device, or after a power loss, the
  //  // following line sets the RTC to the date & time this sketch was compiled
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  // This line sets the RTC with an explicit date & time, for example to set
  //  // January 21, 2014 at 3am you would call:
  //  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //}

  if(!adc.init()){
    Serial.println("Couldn't find ADS1115");
    for(;;); // Don't proceed, loop forever
  }
  adc.setVoltageRange_mV(ADS1115_RANGE_6144);
  adc.setCompareChannels(ADS1115_COMP_0_1); // differential comp A0/A1
  adc.setConvRate(ADS1115_128_SPS);
  // adc.setMeasureMode(ADS1115_CONTINUOUS);
  adc.setMeasureMode(ADS1115_SINGLE);

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.display();
}

void loop() {
  // A13 is half battery voltage
  int analogValue = analogRead(A13);
  int batteryVolts = 2*analogReadMilliVolts(A13);

  // send serial values
  Serial.printf("ADC battery mVolts = %dmV [%d]\n",batteryVolts, analogValue);

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println("BLE power monitor");

  display.print("battery ");
  display.print(batteryVolts);
  display.println("mV");
  
  DateTime now = rtc.now();
  char buf2[] = "YYYYMMDD-hh:mm:ss";
  Serial.println(now.toString(buf2));
  display.print("RTC ");
  display.println(buf2);

  adc.setCompareChannels(ADS1115_COMP_0_1);
  adc.startSingleMeasurement();
  while(adc.isBusy()){ }

  int a2d_voltage = adc.getResult_mV();
  Serial.printf("A2D A01 diff voltage %dmV\n", a2d_voltage);
  display.print("A2D A01 ");
  display.print(a2d_voltage);
  display.println("mV");

  adc.setCompareChannels(ADS1115_COMP_1_GND);
  adc.startSingleMeasurement();
  while(adc.isBusy()){ }

  int a2d_ref = adc.getResult_mV();
  Serial.printf("A2D A1 ref voltage %dmV\n", a2d_ref);
  display.print("A2D A1  ");
  display.print(a2d_ref);
  display.println("mV");

  display.display();
  delay(200);  // delay in between reads for clear read from serial
}

