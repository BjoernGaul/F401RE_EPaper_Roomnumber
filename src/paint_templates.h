#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"


#define COLORED     0
#define UNCOLORED   1

void paintHorizontalShort(Paint paint, Epd epd);
void paintVerticalLong(Paint paint, Epd epd, int yPosPaintScreen);
void paintUpperLeftCorner(Paint paint, Epd epd, int indWeekday, char* date);

void paintTimes(Paint paint, Epd epd, char* date, int indWeekday)
{
    //Grid Times

    paint.SetHeight(55);
    paint.SetWidth(8*50);
    paint.Clear(UNCOLORED);

    //Day
    if(indWeekday == 1){paint.DrawStringAt(1, 0, "Montag", &Font8, COLORED);
    }else if(indWeekday == 2){paint.DrawStringAt(1, 0, "Dienstag", &Font8, COLORED);
    }else if(indWeekday == 3){paint.DrawStringAt(1, 0, "Mittwoch", &Font8, COLORED);
    }else if(indWeekday == 4){paint.DrawStringAt(1, 0, "Donnerstag", &Font8, COLORED);
    }else if(indWeekday == 5){paint.DrawStringAt(1, 0, "Freitag", &Font8, COLORED);
    }else if(indWeekday == 6){paint.DrawStringAt(1, 0, "Samstag", &Font8, COLORED);
    }else if(indWeekday == 0){paint.DrawStringAt(1, 0, "Sonntag", &Font8, COLORED);
    }

    //Date
    paint.DrawStringAt(1, 15, date, &Font8, COLORED);

    paint.DrawHorizontalLine(0, 32, 55, COLORED);

    //Hours
    char schoolHourPaint[3] = "1.";
    int yOffPaint = 10;
    for(int i = 1; i < 12; i++)
    {
        sprintf(schoolHourPaint, "%d.", i); // Format the string with the value of i + 1
        //yOffPaint for double numbers
        if(i > 9)
        {
            yOffPaint = 5;
        }
        paint.DrawStringAt(yOffPaint, (i-1)*32+42, schoolHourPaint, &Font16, COLORED);
        paint.DrawHorizontalLine(0, (i-1)*32+64-1, 55, COLORED);
        if(i != 11)
        {
            paint.DrawHorizontalLine(0, (i-1)*32+64, 55, COLORED);
        }
    }

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 300-paint.GetHeight(), 0+paint.GetWidth(), 300);
    paint.Clear(UNCOLORED);
}

void paintHorizontalShort(Paint paint, Epd epd)
{
    //Hor
    paint.SetHeight(40);
    paint.SetWidth(8*50);
    paint.Clear(UNCOLORED);
    for(int yPosPaint = 25 ; yPosPaint < epd.width; yPosPaint += 34)
    {
        paint.DrawHorizontalLine(0, yPosPaint, 38, COLORED);
    }
}

void paintVerticalLong(Paint paint, Epd epd, int yPosScreen)
{
        //Ver
        paint.SetWidth(8*50);
        paint.SetHeight(1);
        paint.Clear(COLORED);
        epd.Display_Partial_Not_refresh(paint.GetImage(), 0, yPosScreen, 0+paint.GetWidth(), yPosScreen+paint.GetHeight());
}

void paintUpperLeftCorner(Paint paint, Epd epd, int indWeekday, char* date)
{
    paint.SetHeight(55);
    paint.SetWidth(8*4);
    paint.Clear(UNCOLORED);

    //Day
    if(indWeekday == 1){paint.DrawStringAt(2, 0, "Montag", &Font8, COLORED);
    }else if(indWeekday == 2){paint.DrawStringAt(2, 0, "Dienstag", &Font8, COLORED);
    }else if(indWeekday == 3){paint.DrawStringAt(2, 0, "Mittwoch", &Font8, COLORED);
    }else if(indWeekday == 4){paint.DrawStringAt(2, 0, "Donnerstag", &Font8, COLORED);
    }else if(indWeekday == 5){paint.DrawStringAt(2, 0, "Freitag", &Font8, COLORED);
    }else if(indWeekday == 6){paint.DrawStringAt(2, 0, "Samstag", &Font8, COLORED);
    }else if(indWeekday == 0){paint.DrawStringAt(2, 0, "Sonntag", &Font8, COLORED);
    }

    //Date
    paint.DrawStringAt(1, 15, date, &Font8, COLORED);

    paint.DrawHorizontalLine(0, 32, 55, COLORED);
    
    // Display the current information on the e-paper display
    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 300-paint.GetHeight(), 0+paint.GetWidth(), 300);
}

void paintLoadingscreen(Paint paint, Epd epd)
{
    //Loading screen
    paint.SetHeight(300);
    paint.SetWidth(8*8);
    paint.Clear(UNCOLORED);

    paint.DrawStringAt(5, 5, "Loading...", &Font24, COLORED);
    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 0, 0+paint.GetWidth(), 300);

    paint.Clear(UNCOLORED);
    for(int xpos=64; xpos < 400; xpos+=64)
    {
        epd.Display_Partial_Not_refresh(paint.GetImage(), xpos, 0, xpos+paint.GetWidth(), 300);
    }


}