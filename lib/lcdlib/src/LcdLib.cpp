#include "LcdLib.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

void initialiseLCD() {
  lcd.init();
  lcd.backlight();
}

void clearError(uint8_t sensor_num) {
    lcd.setCursor(1, 1 + sensor_num);
    lcd.print(" ");
    lcd.setCursor(8, 1 + sensor_num);
    lcd.print("  ");
    lcd.setCursor(16, 1 + sensor_num);
    lcd.print("    ");
}

void printReadingsToLCD(uint8_t sensor_num, readings reading) {
  if (reading.isError == true) {
    lcd.setCursor(0, 1 + sensor_num);
    lcd.print("Error reading sensor");
  } else {
    clearError(sensor_num);
    lcd.setCursor(0, 1 + sensor_num);
    lcd.print(sensor_num);
    lcd.setCursor(2, 1 + sensor_num);
    lcd.printf("%2.1f%cC", reading.temperature, (char)223);
    lcd.setCursor(10, 1 + sensor_num);
    lcd.print(reading.humidity, 1);
    lcd.print(" %");
  }
}

void printReadingsToLCD(std::map<int, readings> readingMap) {
    lcd.setCursor(0, 0);
    lcd.print("# Temp    Humidity");
    std::map<int, readings>::iterator it = readingMap.begin();
    while(it != readingMap.end())
    {
      printReadingsToLCD(it->first, it->second);
      it++;
    }
}