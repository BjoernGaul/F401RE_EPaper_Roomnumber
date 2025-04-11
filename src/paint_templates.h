#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void paintHorizontalShort(Paint paint, Epd epd);
void paintVerticalLong(Paint paint, Epd epd, int yPosPaintScreen);

void paintTimes(Paint paint, Epd epd)
{
    //Grid Times

    paint.SetHeight(40);
    paint.SetWidth(8*50);
    paintHorizontalShort(paint, epd);

    //Time
    paint.DrawStringAt(0, 0, "11:30", &Font12, COLORED);
    //Date
    paint.DrawStringAt(0, 15, "10.04.25", &Font8, COLORED);

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
        paint.DrawStringAt(yOffPaint, i*35, schoolHourPaint, &Font16, COLORED);
    }

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 300-paint.GetHeight(), 0+paint.GetWidth(), 300);
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
