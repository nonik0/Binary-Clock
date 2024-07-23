/*
  GIT: <a href="https://github.com/jarzebski/Arduino-DS3231">  https://github.com/jarzebski/Arduino-DS3231
>
  GIT: <a href="https://github.com/wayoda/LedControl">  https://github.com/jarzebski/Arduino-DS3231
>
*/

#include <Wire.h>
#include <DS3231.h>
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
 
DS3231 clock;
RTCDateTime dt;
LedControl lc=LedControl(12,11,10,1);

int hours, minutes, seconds;
byte number[10]={B00000000,B01000000,B00100000,B01100000,B00010000,B01010000,B00110000,B01110000,B00001000,B01001000};

void setup()
{
   //Serial.begin(9600);
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,2); // intensity  the brightness of the display. (0..15)
  /* and clear the display */
  lc.clearDisplay(0);
  //lc.setLed(0,row,col,true);

delay (2000);
lc.setRow(0,1,B01010100);
delay (100);
lc.setRow(0,2,B01010100);
delay (100);
lc.setRow(0,3,B01011100);
delay (100);
lc.setRow(0,4,B01010100);
delay (2500);
lc.clearDisplay(0);   
delay (100);

lc.setColumn(0,1,B11111111);
delay (100);
lc.setColumn(0,1,B00000000);
lc.setColumn(0,2,B11111111);
delay (100);
lc.setColumn(0,2,B00000000);
lc.setColumn(0,3,B11111111);
delay (100);
lc.setColumn(0,3,B00000000);
lc.setColumn(0,4,B11111111);
delay (100);
lc.setColumn(0,4,B00000000);
lc.setColumn(0,5,B11111111);
delay (100);
lc.setColumn(0,5,B00000000);
lc.setColumn(0,6,B11111111);
delay (100);
lc.setColumn(0,6,B00000000);


lc.setColumn(0,5,B11111111);
delay (100);
lc.setColumn(0,5,B00000000);
lc.setColumn(0,4,B11111111);
delay (100);
lc.setColumn(0,4,B00000000);
lc.setColumn(0,3,B11111111);
delay (100);
lc.setColumn(0,3,B00000000);
lc.setColumn(0,2,B11111111);
delay (100);
lc.setColumn(0,2,B00000000);
lc.setColumn(0,1,B11111111);
delay (100);
lc.setColumn(0,1,B00000000);

lc.setRow(0,1,B11111111);
delay (200);   
lc.setRow(0,1,B00000000);
lc.setRow(0,2,B11111111);
delay (200);
lc.setRow(0,2,B00000000);
lc.setRow(0,3,B11111111);
delay (200);
lc.setRow(0,3,B00000000);
lc.setRow(0,4,B11111111);
delay (200);
lc.setRow(0,4,B00000000);
lc.setRow(0,3,B11111111);
delay (200);
lc.setRow(0,3,B00000000);
lc.setRow(0,2,B11111111);
delay (200);
lc.setRow(0,2,B00000000);
lc.setRow(0,1,B11111111);
delay (200);
lc.setRow(0,1,B00000000);
delay (600);

  // Initialize DS3231
  clock.begin();
  // Set sketch compiling time
  //clock.setDateTime(__DATE__, __TIME__);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

int menu = 0, up, down;

int hours_one;
int hours_ten;
int minutes_one;
int minutes_ten;
int seconds_one;
int seconds_ten;
  
void loop()
{ 
  if(digitalRead(5)==0){
    delay(300);
    menu++;
    if(menu>3) menu = 0;
    }
    
  if(menu==0){
  dt = clock.getDateTime();
  
  hours =  dt.hour;
  minutes = dt.minute;
  seconds = dt.second;

  if(hours>12) hours = hours - 12;
  if(hours==0) hours = 1;

  hours_one = hours%10;
  hours_ten = hours/10;

  minutes_one = minutes%10;
  minutes_ten = minutes/10;

  seconds_one = seconds%10;
  seconds_ten = seconds/10;

  lc.setColumn(0,1,number[seconds_one]);
  lc.setColumn(0,2,number[seconds_ten]);
  lc.setColumn(0,3,number[minutes_one]);
  lc.setColumn(0,4,number[minutes_ten]);
  lc.setColumn(0,5,number[hours_one]);
  lc.setColumn(0,6,number[hours_ten]);
  }

  if(menu==1){
    if(digitalRead(6)==0){
      delay(300);
      hours++;
      if(hours>=24)hours = 0;
      }
    if(digitalRead(7)==0){
      delay(300);
      hours--;
      if(hours<0)hours = 23;
      }
    hours_one = hours%10;
    hours_ten = hours/10;
    lc.setColumn(0,1,B00000000);
    lc.setColumn(0,2,B00000000);
    lc.setColumn(0,3,B00000000);
    lc.setColumn(0,4,B00000000);
    lc.setColumn(0,5,number[hours_one]);
    lc.setColumn(0,6,number[hours_ten]);
    }
    
  if(menu==2){
    if(digitalRead(6)==0){
      delay(300);
      minutes++;
      if(minutes>=60) minutes = 0;
      }
    if(digitalRead(7)==0){
      delay(300);
      minutes--;
      if(minutes<0) minutes = 59;
      }
    minutes_one = minutes%10;
    minutes_ten = minutes/10;
    lc.setColumn(0,1,B00000000);
    lc.setColumn(0,2,B00000000);
    lc.setColumn(0,5,B00000000);
    lc.setColumn(0,6,B00000000);
    lc.setColumn(0,3,number[minutes_one]);
    lc.setColumn(0,4,number[minutes_ten]);
    }
    
  if(menu==3){
    clock.setDateTime(2024, 1, 10, hours, minutes, 01);
    menu = 0;
    }
  

//  lc.setLed(0,row,col,false);
//  lc.setLed(0,row,col,true);
//  lc.setColumn(0,col,B10100000);
//  lc.setRow(0,4,B11111111);
//  lc.setRow(0,row,(byte)0);
//  lc.setColumn(0,col,(byte)0);

//   For leading zero look to DS3231_dateformat example
//  Serial.print("Raw data: ");
//  Serial.print(dt.year);   Serial.print("-");
//  Serial.print(dt.month);  Serial.print("-");
//  Serial.print(dt.day);    Serial.print(" ");
//  Serial.print(dt.hour);   Serial.print(":");
//  Serial.print(dt.minute); Serial.print(":");
//  Serial.print(dt.second); Serial.println("");
//
//  delay(1000);
}
