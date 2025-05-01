#include <arduino.h>
#include <RTClib.h>
#include <Wire.h>


void getTime(RTC_DS3231 rtc, char* timeBuffer, float* digitTimeBuffer) {
    Serial.println("Getting time...");
    DateTime now = rtc.now();
    sprintf(timeBuffer, "%02d:%02d", now.hour(), now.minute());
    *digitTimeBuffer = now.hour() + (now.minute() / 60.0); // Convert to decimal time
    Serial.print("Decimal time: ");
    Serial.println(*digitTimeBuffer); // Print the decimal time to the Serial Monitor
}

void getDate(RTC_DS3231 rtc, char* dateBuffer) {
    Serial.println("Getting date...");
    DateTime now = rtc.now();
    sprintf(dateBuffer, "%02d.%02d.%02d", now.day(), now.month(), now.year() % 100);
}

int getDayOfWeek(RTC_DS3231 rtc) {
    Serial.println("Getting day of the week...");
    DateTime now = rtc.now();
    Serial.println("Now worked"); // Print the day of the week
    Serial.println(now.dayOfTheWeek()); // Print the day of the week to the Serial Monitor
    return now.dayOfTheWeek(); // Returns 0 for Sunday, 1 for Monday, etc.
}