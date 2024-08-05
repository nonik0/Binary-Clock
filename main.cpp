#include "DS3231.h"
#include <EEPROM.h>
#include "LedControl.h"
#include <Wire.h>

#define EEPROM_ID_ADDR 1023
#define EEPROM_ID 100
#define IS24HOUR_EEPROM_ADDR 0
#define BRIGHTNESS_EEPROM_ADDR 1

#define MENU_PIN 5
#define UP_PIN 6
#define DOWN_PIN 7
#define DATA_PIN 12
#define CLK_PIN 11
#define LOAD_PIN 10

DS3231 rtc;
RTCDateTime dt;
LedControl lc = LedControl(DATA_PIN, CLK_PIN, LOAD_PIN, 1);
const byte number[10] = {B00000000, B01000000, B00100000, B01100000, B00010000, B01010000, B00110000, B01110000, B00001000, B01001000};

enum Mode
{
  Clock = 0,
  SetHours = 1,
  SetMinutes = 2,
  Set24Hour = 3,
  SetBrightness = 4,
  Confirm = 5,
};

int hours, minutes, seconds;

Mode mode = Mode::Clock;

int brightness = 2;
bool is24Hour = true;
int hours_adj; // holds 12/24 hour time while hours always has 24h
int hours_one;
int hours_ten;
int minutes_one;
int minutes_ten;
int seconds_one;
int seconds_ten;

void setup()
{
  // Initialize DS3231
  rtc.begin();

  pinMode(MENU_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  // EEPROM
  if (EEPROM.read(EEPROM_ID_ADDR) != EEPROM_ID) {
    EEPROM.put(EEPROM_ID_ADDR, EEPROM_ID);
    EEPROM.put(IS24HOUR_EEPROM_ADDR, is24Hour);
    EEPROM.put(BRIGHTNESS_EEPROM_ADDR, brightness);
  }
  EEPROM.get(IS24HOUR_EEPROM_ADDR, is24Hour);
  EEPROM.get(BRIGHTNESS_EEPROM_ADDR, brightness);

  /*
 The MAX72XX is in power-saving mode on startup,
 we have to do a wakeup call
 */
  lc.shutdown(0, false);
  lc.setIntensity(0, brightness); // intensity  the brightness of the display. (0..15)
  lc.clearDisplay(0);

  delay(2000);

  // say hi
  lc.setRow(0, 4, B01010100);
  lc.setRow(0, 3, B01011100);
  lc.setRow(0, 2, B01010100);
  lc.setRow(0, 1, B01010100);
  delay(100);
  delay(100);
  delay(100);
  delay(2500);
  lc.clearDisplay(0);
  delay(100);

  // scan columns
  lc.setColumn(0, 1, B11111111);
  delay(100);
  lc.setColumn(0, 1, B00000000);
  lc.setColumn(0, 2, B11111111);
  delay(100);
  lc.setColumn(0, 2, B00000000);
  lc.setColumn(0, 3, B11111111);
  delay(100);
  lc.setColumn(0, 3, B00000000);
  lc.setColumn(0, 4, B11111111);
  delay(100);
  lc.setColumn(0, 4, B00000000);
  lc.setColumn(0, 5, B11111111);
  delay(100);
  lc.setColumn(0, 5, B00000000);
  lc.setColumn(0, 6, B11111111);
  delay(100);
  lc.setColumn(0, 6, B00000000);

  lc.setColumn(0, 5, B11111111);
  delay(100);
  lc.setColumn(0, 5, B00000000);
  lc.setColumn(0, 4, B11111111);
  delay(100);
  lc.setColumn(0, 4, B00000000);
  lc.setColumn(0, 3, B11111111);
  delay(100);
  lc.setColumn(0, 3, B00000000);
  lc.setColumn(0, 2, B11111111);
  delay(100);
  lc.setColumn(0, 2, B00000000);
  lc.setColumn(0, 1, B11111111);
  delay(100);
  lc.setColumn(0, 1, B00000000);

  // scan rows
  lc.setRow(0, 1, B11111111);
  delay(200);
  lc.setRow(0, 1, B00000000);
  lc.setRow(0, 2, B11111111);
  delay(200);
  lc.setRow(0, 2, B00000000);
  lc.setRow(0, 3, B11111111);
  delay(200);
  lc.setRow(0, 3, B00000000);
  lc.setRow(0, 4, B11111111);
  delay(200);
  lc.setRow(0, 4, B00000000);
  lc.setRow(0, 3, B11111111);
  delay(200);
  lc.setRow(0, 3, B00000000);
  lc.setRow(0, 2, B11111111);
  delay(200);
  lc.setRow(0, 2, B00000000);
  lc.setRow(0, 1, B11111111);
  delay(200);
  lc.setRow(0, 1, B00000000);
  delay(600);
}

void loop()
{
  if (digitalRead(MENU_PIN) == 0)
  {
    delay(300);
    mode = (Mode)(mode + 1);
  }

  if (mode == Mode::Clock)
  {
    dt = rtc.getDateTime();

    hours = dt.hour;
    minutes = dt.minute;
    seconds = dt.second;

    hours_adj = is24Hour ? hours : (hours + 11) % 12 + 1; 

    hours_one = hours_adj % 10;
    hours_ten = hours_adj / 10;

    minutes_one = minutes % 10;
    minutes_ten = minutes / 10;

    seconds_one = seconds % 10;
    seconds_ten = seconds / 10;

    lc.setColumn(0, 1, number[seconds_one]);
    lc.setColumn(0, 2, number[seconds_ten]);
    lc.setColumn(0, 3, number[minutes_one]);
    lc.setColumn(0, 4, number[minutes_ten]);
    lc.setColumn(0, 5, number[hours_one]);
    lc.setColumn(0, 6, number[hours_ten]);
  }

  else if (mode == Mode::SetHours)
  {
    // TODO: maybe limit to 12 hours if is24Hour is false?
    if (digitalRead(UP_PIN) == 0)
    {
      delay(300);
      hours++;
      if (hours >= 24)
        hours = 0;
    }
    if (digitalRead(DOWN_PIN) == 0)
    {
      delay(300);
      hours--;
      if (hours < 0)
        hours = 23;
    }
    hours_one = hours % 10;
    hours_ten = hours / 10;
    lc.setColumn(0, 1, B00000000);
    lc.setColumn(0, 2, B00000000);
    lc.setColumn(0, 3, B00000000);
    lc.setColumn(0, 4, B00000000);
    lc.setColumn(0, 5, number[hours_one]);
    lc.setColumn(0, 6, number[hours_ten]);
  }

  else if (mode == Mode::SetMinutes)
  {
    if (digitalRead(UP_PIN) == 0)
    {
      delay(300);
      minutes++;
      if (minutes >= 60)
        minutes = 0;
    }
    if (digitalRead(DOWN_PIN) == 0)
    {
      delay(300);
      minutes--;
      if (minutes < 0)
        minutes = 59;
    }
    minutes_one = minutes % 10;
    minutes_ten = minutes / 10;
    lc.setColumn(0, 1, B00000000);
    lc.setColumn(0, 2, B00000000);
    lc.setColumn(0, 5, B00000000);
    lc.setColumn(0, 6, B00000000);
    lc.setColumn(0, 3, number[minutes_one]);
    lc.setColumn(0, 4, number[minutes_ten]);
  }

  else if (mode == Mode::Set24Hour)
  {
    {
      if (digitalRead(UP_PIN) == 0)
      {
        delay(300);
        is24Hour = !is24Hour;
      }
      if (digitalRead(DOWN_PIN) == 0)
      {
        delay(300);
        is24Hour = !is24Hour;
      }
      int hoursMode = is24Hour ? 24 : 12;
      hours_one = hoursMode % 10;
      hours_ten = hoursMode / 10;
      lc.setColumn(0, 1, B00000000);
      lc.setColumn(0, 2, B00000000);
      lc.setColumn(0, 3, B00000000);
      lc.setColumn(0, 4, B00000000);
      lc.setColumn(0, 5, number[hours_one]);
      lc.setColumn(0, 6, number[hours_ten]);
    }
  }

  else if (mode == Mode::SetBrightness)
  {
    if (digitalRead(UP_PIN) == 0)
    {
      delay(300);
      brightness++;
      if (brightness > 3)
        brightness = 0;
      lc.setIntensity(0, brightness);
    }
    if (digitalRead(DOWN_PIN) == 0)
    {
      delay(300);
      brightness--;
      if (brightness < 0)
        brightness = 3;
      lc.setIntensity(0, brightness);
    }

    unsigned char brightIndicator = B00000000;
    for(int i = 0; i < brightness + 1; i++)
      brightIndicator = (brightIndicator >> 1) | B01000000;
    lc.setColumn(0, 1, B01111000 & brightIndicator);
    lc.setColumn(0, 2, B01110000 & brightIndicator);
    lc.setColumn(0, 3, B01100000 & brightIndicator);
    lc.setColumn(0, 4, B01000000 & brightIndicator);
    lc.setColumn(0, 5, B00000000);
    lc.setColumn(0, 6, B00000000);
  }

  else if (mode == Mode::Confirm)
  {
    rtc.setDateTime(2024, 7, 29, hours, minutes, 01);
    EEPROM.put(IS24HOUR_EEPROM_ADDR, is24Hour);
    EEPROM.put(BRIGHTNESS_EEPROM_ADDR, brightness);
    mode = Mode::Clock;
  }
}