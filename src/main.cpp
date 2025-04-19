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

#define COLORED     0
#define UNCOLORED   1

#define sdcsPin PD_8

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
    Serial.begin(9600);
    Epd epd;
    Serial.println("Starting");

    delay(100);

    if(!SD.begin(sdcsPin))
    {
        Serial.println("SD-init failed");
        return;
    }
    delay(100);

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


    unsigned char image[20000];
    Paint paint(image, 8*50, 1);    //width should be the multiple of 8 
    paint.SetRotate(3);

    //paintHourplan(paint, epd);
    paintHourplanDay(paint, epd, dataSchoolhoursbuffer, MONDAY);

    epd.TurnOnDisplay_Partial();

    epd.Init();
    delay(1000);

    /* Deep sleep */
    epd.Sleep();


}

void loop() {
  // put your main code here, to run repeatedly:

}
