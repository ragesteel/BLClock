/*
Basic Led Clock, часы на адресных светодиодах
*/

#define LED_PIN       6  // Контакт для вывода информации на адресные светодиоды
#define LED_PER_SEG   2  // Количество светодиодов на сегмент
#define LED_COUNT 4 * 7 * LED_PER_SEG + 2  // Общее кол-во светодиодов

#include <microLED.h> // 3.6.0

microLED <LED_COUNT, LED_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB> display;


void setup() {
  display.setBrightness(60);
}

void loop() {
  static byte counter = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    display.set(i, mWheel8(counter + i * 255 / LED_COUNT));
  }
  counter += 3;
  delay(30);
}
