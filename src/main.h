#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_SPACE_SYMBOL 0x20
#define MERAH 13
#define KUNING 12
#define HIJAU 11
#define MULTITASKING_STATUS 10

float getCelcius();
float getFahren();
float getReamur();
float getKelvin();
String checkAlarm();
void cetakSuhu();
void setAlarm(uint8_t pin, uint16_t durasi);

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);