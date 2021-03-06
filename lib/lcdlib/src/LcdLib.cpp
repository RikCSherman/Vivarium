#include "LcdLib.h"

#include <queues.h>
#include <utils.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void clearError(uint8_t sensor_num) {
    lcd.setCursor(1, 1 + sensor_num);
    lcd.print(" ");
    lcd.setCursor(8, 1 + sensor_num);
    lcd.print("  ");
    lcd.setCursor(16, 1 + sensor_num);
    lcd.print("    ");
}

void printReadingsToLCD(uint8_t sensor_num, Reading reading) {
    clearError(sensor_num);
    lcd.setCursor(0, 1 + sensor_num);
    lcd.print(sensor_num);
    lcd.setCursor(2, 1 + sensor_num);
    lcd.print(reading.temperature, 1);
    lcd.printf("%cC", (char)223);
    lcd.setCursor(10, 1 + sensor_num);
    lcd.print(reading.humidity, 1);
    lcd.print(" %");
}

void printReadingsToLCD(Readings readings) {
    lcd.setCursor(0, 0);
    lcd.print("# Temp    Humidity");
    printReadingsToLCD(0, readings.bme1);
    // printReadingsToLCD(1, readings.bme2);
}

void receive_Reading(void *argument) {
    Readings received;
    while (true) {
        if (xQueueReceive(lcdQueue, &received, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from lcd Queue");
        } else {
            printReadingsToLCD(received);
        }
        taskYIELD();
    }
}

void initialiseLCD() {
    lcd.init();
    lcd.backlight();
    xTaskCreatePinnedToCore(receive_Reading,           // Function that should be called
                            "Receive Reading to LCD",  // Name of the task (for debugging)
                            5000,                      // Stack size (bytes)
                            NULL,                      // Parameter to pass
                            3,                         // Task priority
                            NULL,                      // Task handle
                            1);
}