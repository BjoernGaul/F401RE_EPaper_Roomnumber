#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"
#include "sd_reader.h"

#define COLORED     0
#define UNCOLORED   1


void paintHourplanDay(Paint paint, Epd epd, int weekDay)
{
    int dayLine = 0;
    String weekDayString = "";


    switch (weekDay)
    {
    case 1:
        weekDayString = "Montag";
        dayLine = 2;
        break;
    
    case 2:
        weekDayString = "Dienstag";
        dayLine = 15;
        break;
    
    case 3:
        weekDayString = "Mittwoch";
        dayLine = 28;
        break;
    
    case 4:
        weekDayString = "Donnerstag";
        dayLine = 41;
        break;
    
    case 5:
        weekDayString = "Freitag";
        dayLine = 54;
        break;
    
    default:
        break;
    }

    char charArray[50];
    int partsInd = 1;

    //Grid
    paint.Clear(COLORED);
    for(int yPosScreen = 259 ; yPosScreen > 0; yPosScreen -= 51 )
    {
        //Basic Grid
        paint.SetHeight(40);
        paint.SetWidth(8*50);
        paintHorizontalShort(paint, epd);

        if(partsInd < 4)
        {
            for(int row = 0; row < 12; row++)
            {
                // Data
                String line = readSDLine(dayLine); // Example: Read a line from SD card
                String part = getPartByIndex(line, partsInd);      // Extract the first part (subject)
                part.toCharArray(charArray, sizeof(charArray)); //make String to char so Drawstring accepts it

                Serial.println(part);

                paint.DrawStringAt(0, 0, charArray, &Font16, COLORED);
    
                dayLine++;
                delay(5);
            }
        }
        epd.Display_Partial_Not_refresh(paint.GetImage(), 0, yPosScreen-paint.GetHeight()-5, 0+paint.GetWidth(), yPosScreen-5);
        partsInd++;
    }
    paintTimes(paint, epd);
}



