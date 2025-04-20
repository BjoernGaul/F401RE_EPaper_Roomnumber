#include <arduino.h>
#include <RTClib.h>
#include <Wire.h>

void initRTC(RTC_DS3231 rtc)
{
    Serial.println("Initializing RTC...");
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt if RTC is not found
    }
}

void getTime(RTC_DS3231 rtc, char* timeBuffer) {
    DateTime now = rtc.now();
    sprintf(timeBuffer, "%02d:%02d", now.hour(), now.minute());
}

void getDate(RTC_DS3231 rtc, char* dateBuffer) {
    DateTime now = rtc.now();
    sprintf(dateBuffer, "%02d.%02d.%02d", now.day(), now.month(), now.year() % 100);
}

int getDayOfWeek(RTC_DS3231 rtc) {
    DateTime now = rtc.now();
    return now.dayOfTheWeek(); // Returns 0 for Sunday, 1 for Monday, etc.
}