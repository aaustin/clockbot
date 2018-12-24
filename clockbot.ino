#include <Wire.h>
#include <DS3231.h>

RTClib RTC;

int lastHourSetting, lastMinuteSetting;

int allPins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52};

int hourPins[][3] = {
  { 45, 46, 47 }, // twelve
  { 23, 24, -1 }, // one
  { 34, 35, -1 }, // two
  { 27, 28, 29 }, // three
  { 30, 31, -1 }, // four
  { 32, 33, -1 }, // five
  { 25, 26, -1 }, // six
  { 42, 43, 44 }, // seven
  { 36, 37, 38 }, // eight
  { 13, 22, -1 }, // nine
  { 48, 49, -1 }, // ten
  { 39, 40, 41 } // eleven
};

int minutePins[][7] = {
  { 50, 51, 52, -1, -1, -1, -1 }, // oclock
  { 11, 12, 19, 3, -1, -1, -1 }, // five past
  { 16, 17, 19, 3, -1, -1, -1 }, // ten past
  { 4, 5, 6, 7, 19, 3, -1 }, // a quarter past
  { 8, 9, 10, 19, 3, -1, -1 }, // twenty past
  { 8, 9, 10, 11, 12, 19, 3 }, // twentyfive past
  { 14, 15, 19, 3, -1, -1, -1 }, // half past
  { 8, 9, 10, 11, 12, 18, -1 }, // twentyfive to
  { 8, 9, 10, 18, -1, -1, -1 }, // twenty to
  { 4, 5, 6, 7, 18, -1, -1 }, // a quarter to
  { 16, 17, 18, -1, -1, -1, -1 }, // ten to
  { 11, 12, 18, -1, -1, -1, -1 } // five to
};

// used to calibrate the time
//byte ss=0,   mi=27,   hh=4,   wd=6,    dd=22,   mo=12,   yy=19;
//DS3231 clock = DS3231();
//clock.setClockMode(true);
//clock.setSecond(ss);
//clock.setMinute(mi);
//clock.setHour(hh);
//clock.setDoW(wd);
//clock.setDate(dd);
//clock.setMonth(mo);
//clock.setYear(yy);

void setup() {
  Wire.begin();
  
  lastHourSetting = -1;
  lastMinuteSetting = 1;
  
  for (int i = 0; i < sizeof(allPins)/sizeof(int); i++) {
    // pinMode(allPins[i], INPUT);
    pinMode(allPins[i], OUTPUT);
    digitalWrite(allPins[i], LOW);
  }
}

void updateLEDs(int hourIndex, int minuteIndex) {
  for (int i = 0; i < sizeof(allPins)/sizeof(int); i++) {
    digitalWrite(allPins[i], LOW);
  }
  delay(1000);
  // IT IS
  digitalWrite(2, HIGH);
  // SET THE HOUR
  for (int i = 0; i < sizeof(hourPins[hourIndex])/sizeof(int); i++) {
    if (hourPins[hourIndex][i] > 0) {
      digitalWrite(hourPins[hourIndex][i], HIGH);
    }
  }
  // SET THE MINUTE
  for (int i = 0; i < sizeof(minutePins[minuteIndex])/sizeof(int); i++) {
    if (minutePins[minuteIndex][i] > 0) {
      digitalWrite(minutePins[minuteIndex][i], HIGH);
    }
  }
}

int calculateMinuteIndex() {
  DateTime now = RTC.now();
  int minuteAdj = (now.minute() + 2) % 60;
  return (minuteAdj / 5);
}

int calculateHourIndex(int minuteIndex) {
  DateTime now = RTC.now();
  int hourAdj = now.hour() % 12;
  if (minuteIndex > 6) {
    return hourAdj + 1;
  } else if (minuteIndex == 0 && now.minute() > 55) {
    return hourAdj + 1;
  }
  return hourAdj;
}

void loop() {
  int currMinuteIndex = calculateMinuteIndex();
  int currHourIndex = calculateHourIndex(currMinuteIndex);

  if (currMinuteIndex != lastMinuteSetting || currHourIndex != lastHourSetting) {
    updateLEDs(currHourIndex, currMinuteIndex);
    lastMinuteSetting = currMinuteIndex;
    lastHourSetting = currHourIndex; 
  }

  delay(30000);
}
