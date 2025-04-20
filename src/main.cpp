#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "imagedata.h"
#include "epdpaint.h"
//  #include "paint_templates.h"
#include "screen_week.h"
#include "screen_day.h"
#include "SD.h"
#include "sd_reader.h"
#include "Wire.h"
#include "rtc_datetime.h"

Epd epd;
unsigned char image[20000];
Paint paint(image, 8*50, 1);    //width should be the multiple of 8 

RTC_DS3231 rtc; // Create an RTC object
int weekday = 0; // Variable to store the current day of the week
char Date[15]; // Buffer to store the date string
char Time[10]; // Buffer to store the time string

#define COLORED     0
#define UNCOLORED   1

#define sdcsPin PD_8
#define ButtonPin PC13


bool ButtonPressed = false; // Flag to indicate button press state
int statePaper = 0; // State variable to track the current state of the paper

volatile unsigned long lastInterruptTime = 0;
uint64_t lastMillisTimeTest = 0; // Variable to store the last time the button was pressed




void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Starting");

    pinMode(ButtonPin, INPUT_PULLUP); // Set the button pin as input with pull-up resistor
    attachInterrupt(digitalPinToInterrupt(ButtonPin), []() {
        unsigned long interruptTime = millis();
        if (interruptTime - lastInterruptTime > 200) { // 200ms debounce time
            ButtonPressed = true;
            Serial.println("Button pressed!");
        }
        lastInterruptTime = interruptTime;
    }, FALLING); // Trigger on falling edge (button press)


    Wire.begin(SCL,SDA);

    Serial.println("Scanning...");

    for (uint8_t address = 1; address < 127; address++) { // I2C addresses range from 0x01 to 0x7F
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0"); // Add leading zero for single-digit addresses
            Serial.println(address, HEX);
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    Serial.println("Scan complete.");
    delay(5000); // Wait 5 seconds before scanning 

    delay(50);
    initRTC(rtc); // Initialize the RTC module
    weekday = getDayOfWeek(rtc); // Get the current day of the week
    getDate(rtc, Date); // Get the current date
    getTime(rtc, Time); // Get the current time


    pinMode(PD_10, OUTPUT); // Set PD_10 as output for CS pin
    digitalWrite(PD_10, HIGH); // Set PD_10 to HIGH (inactive state)

    pinMode(PD_8, OUTPUT); // Set PD_8 as output for SD card CS pin
    digitalWrite(PD_8, LOW); // Set PD_8 to LOW (active state)

    delay(500);

    if (!initSDCard()) {
      Serial.println("SD card initialization failed. Halting...");
      while (true); // Halt execution
    }
    delay(500);

    readAllSDData();

    Serial.println("SD-init successfull");
    delay(100);


    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    //Serial.print(UNCOLORED);
    /* This clears the SRAM of the e-paper display */
    epd.Clear();

    paint.SetRotate(3);

    paintHourplan(paint, epd, dataSchoolhoursbuffer, Date, weekday);

    epd.TurnOnDisplay_Partial(); // Turn on the display with fast refresh

    epd.Init();
    epd.Sleep(); // Put the display to sleep to save power

}

void loop() {
    if (ButtonPressed) { // Check if the button was pressed or if 10 seconds have passed
        if(statePaper >= 1){
            statePaper = 0; // Reset to the first state if it exceeds the number of states
        } else {
            statePaper++; // Increment the state variable
        }
        switch (statePaper) 
        {
            case 0:
                Serial.println("State 0: Full Hourplan");
                lastMillisTimeTest = millis();
                epd.Init();
                epd.Clear();
                paintHourplan(paint, epd, dataSchoolhoursbuffer, Date, weekday);
                epd.TurnOnDisplay_Partial();
                epd.Sleep(); // Put the display to sleep to save power
            break;
        
            case 1:
                Serial.println("State 1: Weekday Hourplan");
                lastMillisTimeTest = millis();
                epd.Init();
                epd.Clear();
                paintHourplanDay(paint, epd, dataSchoolhoursbuffer, weekday, Date);
                epd.TurnOnDisplay_Partial();
                epd.Sleep(); // Put the display to sleep to save power         
            break;

            default:
            break;
        }
        ButtonPressed = false;
    }
}
