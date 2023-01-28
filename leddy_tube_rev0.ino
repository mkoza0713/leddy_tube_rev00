#include "RTClib.h"
RTC_DS1307 rtc;
byte nowHour;
byte nowMinute;

byte lockRtc = 1;
byte lightModeHour = 0;  //tryb pracy wysyalny do funkcji
byte prevLightModeHour = 0;
byte lockModeStage = 0;  //żeby nie zmieniać bez potrzeby stanu cały czas
#define relay1 8


void setup () {
  Serial.begin(9600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    lockRtc=0; //RTC zablokowane
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //rtc.adjust(DateTime(2023, 1, 28, 20, 16, 0));

  pinMode(relay1, OUTPUT);

}

void loop () {
    DateTime now = rtc.now();

    if(lockRtc==1){
      hourStage();
      if(prevLightModeHour!=lightModeHour){
        lockModeStage=0;
      }
      if(lockModeStage==0){
        lockModeStage=1;
        prevLightModeHour=lightModeHour;
        lightMode(lightModeHour);
      }
    }

    nowHour = now.hour();
    nowMinute = now.minute();
    // Serial.print(now.hour());
    // Serial.print(':');
    // Serial.print(now.minute());
    // Serial.print(':');
    // Serial.print(now.second());
    // Serial.print("   prev Tryb pracy:");
    // Serial.print(lightModeHour);
    // Serial.print("   Tryb pracy:");
    // Serial.println(lightModeHour);

}
void hourStage(){
  byte morningStartH=7;
  byte morningStartM=0;
  byte morningStopH=8;
  byte morningStopM=0;

  byte afternoonStartH=8;
  byte afternoonStartM=0;
  byte afternoonStoptH=15;
  byte afternoonStoptM=0;

  byte eveningStartH =15 ;
  byte eveningStartM = 0;
  byte eveningStopH = 17;
  byte eveningStopM = 0;

  if(nowHour>=morningStartH && nowMinute>=morningStartM && nowHour<=morningStopH && nowMinute<morningStopM){
    lightModeHour=1;
  }
  else if(nowHour>=afternoonStartH && nowMinute>=afternoonStartM && nowHour<=afternoonStoptH && nowMinute<afternoonStoptM){
    lightModeHour=2;
  }
  else if(nowHour>=eveningStartH && nowMinute>=eveningStartM && nowHour<=eveningStopH && nowMinute<eveningStopM){
    lightModeHour=3;
  }
  else lightModeHour=0;
}
void lightMode(byte mode){
  digitalWrite(relay1, LOW);
  delay(20000);  //czekam zeby swietlowka zaczela od domyslnego
  switch(mode){
    case 1:  //swiecenie poranne
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
    break;
    case 2://swiecenie dzienne
      digitalWrite(relay1, HIGH);
      delay(1000);
    break;
    case 3://swiecenie wieczorne
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
    break;
    case 4://swiecenie nocne
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
      delay(1000);
    break;
  }
}
