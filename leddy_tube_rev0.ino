#include "RTClib.h"
RTC_DS1307 rtc;
unsigned int nowHour;
unsigned int nowMinute;
unsigned int nowSec;
unsigned long timeNow;

byte lockRtc = 1;
byte lightModeHour = 0;  //tryb pracy wysyalny do funkcji
byte prevLightModeHour = 0;
byte lockModeStage = 0;  //żeby nie zmieniać bez potrzeby stanu cały czas
#define relay1 8


void setup() {
  Serial.begin(9600);
  Serial.println("START SYSTEMU");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    lockRtc = 0;  //RTC zablokowane
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //rtc.adjust(DateTime(2023, 1, 28, 20, 16, 0));

  pinMode(relay1, OUTPUT);
}

void loop() {
  DateTime now = rtc.now();

  if (lockRtc == 1) {
    hourStage();
    if (prevLightModeHour != lightModeHour) {
      lockModeStage = 0;
    }
    if (lockModeStage == 0) {
      lockModeStage = 1;
      prevLightModeHour = lightModeHour;
      lightMode(lightModeHour);
    }
  }

  nowHour = now.hour();
  nowMinute = now.minute();
  nowSec = now.second();
  
  timeNow = (nowHour*(long)3600) + (nowMinute*60)+nowSec;  //tyle sekund minelo od polnocy
  Serial.print("Tyle od polnocy: ");
  Serial.print(timeNow);
  Serial.print("  ");
  Serial.print(nowHour);
  Serial.print(':');
  Serial.print(nowMinute);
  Serial.print(':');
  Serial.print(nowSec);
  Serial.print("   lightModeHour: ");
  Serial.println(lightModeHour);

}
void hourStage() {
  long startTime1;
  long startTime2;
  long startTime3;
  long startTime4;
  long stopTime1;
  long stopTime2;
  long stopTime3;
  long stopTime4;

  byte morningStartH = 7;
  byte morningStartM = 0;
  byte morningStopH = 8;
  byte morningStopM = 0;
  startTime1=morningStartH*(long)3600+morningStartM*60;
  stopTime1=morningStopH*(long)3600+morningStopM*60;

  byte afternoonStartH = 8;
  byte afternoonStartM = 0;
  byte afternoonStoptH = 15;
  byte afternoonStoptM = 0;
  startTime2=afternoonStartH*(long)3600+afternoonStartM*60;
  stopTime2=afternoonStoptH*(long)3600+afternoonStoptM*60;

  byte eveningStartH = 15;
  byte eveningStartM = 0;
  byte eveningStopH = 17;
  byte eveningStopM = 0;
  startTime3=eveningStartH*(long)3600+eveningStartM*60;
  stopTime3=eveningStopH*(long)3600+eveningStopM*60;

  byte nightStartH = 17;
  byte nightStartM = 0;
  byte nightStopH = 19;
  byte nightStopM = 0;
  startTime4=nightStartH*(long)3600+nightStartM*60;
  stopTime4=nightStopH*(long)3600+nightStopM*60;

  if (timeNow >= startTime1 && timeNow<= stopTime1) {
    lightModeHour = 1;
  } else if (timeNow >= startTime2 && timeNow<= stopTime2 ) {
    lightModeHour = 2;
  } else if (timeNow >= startTime3 && timeNow<= stopTime3 ) {
    lightModeHour = 3;
  } else if (timeNow >= startTime4 && timeNow<= stopTime4 ) {
    lightModeHour = 4;
  } else {
    lightModeHour = 0;
  }

}
void lightMode(byte mode) {
  digitalWrite(relay1, LOW);
  delay(20000);  //czekam zeby swietlowka zaczela od domyslnego
  switch (mode) {
    case 1:  //swiecenie poranne
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
      break;
    case 2:  //swiecenie dzienne
      digitalWrite(relay1, HIGH);
      delay(1000);
      break;
    case 3:  //swiecenie wieczorne
      digitalWrite(relay1, HIGH);
      delay(1000);
      digitalWrite(relay1, LOW);
      delay(1000);
      digitalWrite(relay1, HIGH);
      break;
    case 4:  //swiecenie nocne
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
