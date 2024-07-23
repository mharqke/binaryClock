// #include <avr/wdt.h>
// #include <avr/power.h>
// #include <avr/sleep.h>
#include <GyverOLED.h>
#include "RTClib.h"


RTC_DS3231 rtc;
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

// volatile bool f = 0;




void lines(){    //draw lines
  for(uint8_t i = 0; i < 6; i++){
    oled.line(i*21, 0, i*21, 64, 1);   //vertical
    oled.line(0, i*16, 128, i*16, 1);  //horizontal
  }
  oled.line(127, 0, 127, 64, 1);      //last 
  oled.line(0, 63, 128, 63, 1);       //last        
}

void setup() {
  Serial.begin(57600);
  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  oled.init(2, 3);  // initialization
  oled.setContrast(1); 
  lines();
  oled.update();
  clock_prescale_set(clock_div_8);  //change update time
  Serial.end();
}


void draw(int8_t val, uint8_t x){
  for (int8_t i = 0; i < 4; i++){
    if (bitRead(val, i) == 1){
      oled.rect(x + 2, 16 * (3 - i) + 2, x + 19, 14 + 16 * (3 - i), 1);
    }else{
      oled.rect(x + 2, 16 * (3 - i) + 2, x + 19, 14 + 16 * (3 - i), 0);
    }
  }
}


void btime(uint8_t hour, uint8_t min, uint8_t sec){  //binary time
  draw(hour / 10, 0);
  draw(hour % 10, 21);
  draw(min / 10, 42);
  draw(min % 10, 63);
  draw(sec / 10, 84);
  draw(sec % 10, 105);
}


void loop() {
  
  DateTime now = rtc.now();
  btime(now.hour(), now.minute(), now.second());
  oled.update();

}