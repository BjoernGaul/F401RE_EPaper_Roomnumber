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

Epd epd;
unsigned char image[20000];
Paint paint(image, 8*50, 1);    //width should be the multiple of 8 

#define COLORED     0
#define UNCOLORED   1

#define sdcsPin PD_8
#define ButtonPin PC_13

bool ButtonPressed = false; // Flag to indicate button press state
int statePaper = 0; // State variable to track the current state of the paper

volatile unsigned long lastInterruptTime = 0;

// Define the enum
enum weekDay {
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY
};



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

    paintHourplan(paint, epd, dataSchoolhoursbuffer);
    //paintHourplanDay(paint, epd, dataSchoolhoursbuffer, MONDAY);

    epd.TurnOnDisplay_Partial();

}

void loop() {
    if (ButtonPressed) {
        if(statePaper >= 2){
            statePaper = 0; // Reset to the first state if it exceeds the number of states
        } else {
            statePaper++; // Increment the state variable
        }
        switch (statePaper) 
        {
            case 0:
                Serial.println("State 0: Full Hourplan");
                epd.Clear();
                paintHourplan(paint, epd, dataSchoolhoursbuffer);
                epd.TurnOnDisplay_Partial();
            break;
        
            case 1:
                Serial.println("State 1: Weekday Hourplan");
                epd.Clear();
                paintHourplanDay(paint, epd, dataSchoolhoursbuffer, MONDAY);
                epd.TurnOnDisplay_Partial();
            break;

            default:
            break;
        }
        ButtonPressed = false;
    }
}
