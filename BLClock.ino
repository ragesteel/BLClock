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

#define CH_EMPTY 10

#define MAX_CH 11
byte DIGITS[MAX_CH] = {
//   cdefabg  
  0b01111110, // 0
  0b01000010, // 1
  0b00110111, // 2
  0b01100111, // 3
  0b01001011, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b01000110, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b00000000, // CH_EMPTY
};

void setup() {
  display.setBrightness(60);
  display.clear();
  display.setCLI(CLI_AVER);
  Serial.begin(9600);
}

void loop() {
  static byte counter = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    display.set(i, mWheel8(counter + i * 255 / LED_COUNT));
  }
  counter += 3;

  static byte n = 0;
  static uint32_t last_update = 0;
  if (millis() - last_update > 500) {
    last_update = millis();
    n++;
    if (n > MAX_CH) {
      n = 0;
    }
  }
  setDigit(3, n);
  
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
  /*
  Serial.print("Digit: ");
  Serial.print(digit);
  Serial.print(", 0x1: ");
  Serial.print(digit & 0x1);
  Serial.print(", value: ");
  Serial.print(value);
  Serial.println();
  */
  for (byte i = 0; i < LED_SEGMENT; i++) {
    mData color;
    if ((digit & 0x1) == 0x1) {
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
