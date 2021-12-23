#include <main.h>

void setup()
{
  Serial.begin(9600);

  pinMode(A0, INPUT); //LM35
  pinMode(MERAH, OUTPUT); //Merah
  pinMode(KUNING, OUTPUT); //Kuning
  pinMode(HIJAU, OUTPUT); //Hijau
  pinMode(MULTITASKING_STATUS, OUTPUT); //Lampu tanda kode multitasking

  while (lcd.begin(16, 2) != 1)
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }

  lcd.print(F("KELOMPOK 4"));
  lcd.setCursor(0,2);
  lcd.print(F("INITIALIZING ......"));
  delay(2000);

  lcd.clear();


  lcd.print("STATUS: ");

  lcd.setCursor(7,2);
  lcd.print(F("-"));
 
}


uint16_t loopBefore = millis();
void loop()
{
  lcd.setCursor(0, 2);
  lcd.print(getCelcius());
  lcd.print("C");

  lcd.setCursor(9,2);
  lcd.print(getFahren());
  lcd.print("F ");
  delay(500);
  lcd.write(LCD_SPACE_SYMBOL);
  
  lcd.setCursor(0,2);
  lcd.print(getReamur());
  lcd.print("R");
  
  lcd.setCursor(9,2);
  lcd.print(getKelvin());
  lcd.print("K");
  delay(500);
  lcd.write(LCD_SPACE_SYMBOL);
  
  lcd.setCursor(7, 0);
  lcd.print(checkAlarm());
  lcd.write(LCD_SPACE_SYMBOL);
  
  uint16_t now = millis();
  if( (now - loopBefore) > 50 )
  {
    digitalWrite(MULTITASKING_STATUS, !digitalRead(MULTITASKING_STATUS));
    loopBefore = now;
  }


  cetakSuhu();
  checkAlarm();
}

String checkAlarm()
{
  float celcius = getCelcius();

  if(celcius >= 18 && celcius <= 30)
  {
    digitalWrite(MERAH, LOW);
    digitalWrite(KUNING, LOW);
    setAlarm(HIJAU, 100);
    return "NORMAL";
  }
  else if(celcius > 30 && celcius <= 40)
  {
    digitalWrite(MERAH, LOW);
    digitalWrite(HIJAU, LOW);
    setAlarm(KUNING, 100);
    return "AWAS  ";
  }
  else if(celcius > 40)
  {
    digitalWrite(HIJAU, LOW);
    digitalWrite(KUNING, LOW);
    setAlarm(MERAH, 100);
    return "BAHAYA";
  }
  else
  {
    digitalWrite(MERAH, LOW);
    digitalWrite(KUNING, LOW);
    digitalWrite(HIJAU, LOW);
    return "UNKNOWN";
  
  }
}

uint16_t setAlarmBefore = millis();
void setAlarm(uint8_t pin, uint16_t durasi)
{
  uint16_t now = millis();
  if( (now - setAlarmBefore) > durasi )
  {
    digitalWrite(pin, !digitalRead(pin));
    setAlarmBefore = now;
  }
}

uint16_t cetakSuhuBefore = millis();
void cetakSuhu()
{
  uint16_t now = millis();
  if( (now - cetakSuhuBefore) > 500 )
  {
    char buffer[64];
    sprintf_P(buffer, PSTR("%sC - %sF - %sR - %sK (%s)"),
      String(getCelcius()).c_str(), String(getFahren()).c_str(),
      String(getReamur()).c_str(), String(getKelvin()).c_str(), checkAlarm().c_str());
    Serial.println(buffer);
    cetakSuhuBefore = now;
  }
}

float getCelcius()
{
  int sinyalOutputTerkonversi = analogRead(A0);
  float sinyalOutput = (sinyalOutputTerkonversi/1023.0) * 5000;
  float celcius = (sinyalOutput / 10.0);

  return celcius;
}

float getFahren()
{
  float fahren = (getCelcius() * 1.8) + 32;
  return fahren;
}

float getReamur()
{
  float reamur = getCelcius() * 0.8;
  return reamur;
}

float getKelvin()
{
  float kelvin = getCelcius() + 273.0;
  return kelvin;
}

