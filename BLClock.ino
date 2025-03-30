/*
Basic Led Clock, часы на адресных светодиодах
*/

#define LED_PIN       6  // Контакт для вывода информации на адресные светодиоды
#define LED_PER_SEG   2  // Количество светодиодов на сегмент
#define LED_SEGMENT   7  // Количество сегментов
#define LED_COUNT 4 * LED_SEGMENT * LED_PER_SEG + 2  // Общее кол-во светодиодов

#include <microLED.h> // 3.6.0

microLED <LED_COUNT, LED_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB> display;

// Определение цифр и прочих символов
//  aa
// f  b
//  gg
// e  c
//  dd  

byte DIGITS[1] = {
  0b01111110 // 0
};

void setup() {
  display.setBrightness(60);
  display.clear();
}

void loop() {
  static byte counter = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    display.set(i, mWheel8(counter + i * 255 / LED_COUNT));
  }
  counter += 3;
  setDigit(3, 0);
  display.show();
  delay(30);
}

// Установить цифру на заданной позиции
// position 0 — дестяки часов, 1 — единицы часов, 2 — десятки минут, 3 — единицы минут
// value — значение
void setDigit(byte position, byte value) {

  // Найдём начальный светодиод
  byte start = position * LED_SEGMENT * LED_PER_SEG;
  if (position > 1) {
    // Для минут — сдвиг на два светодиода, образующих двоеточие
    start += 2;
  }

  byte digit = DIGITS[value];
  for (byte i = 0; i < LED_SEGMENT; i++) {
    mData color;
    if ((digit && 0x1) == 0x1) {
      color = mGreen;
    } else {
      color = mBlack;
    }
    for (byte s = 0; s < LED_PER_SEG; s++) {
      display.set(start++, color);
    }
    digit = digit >> 1;
  }
}
