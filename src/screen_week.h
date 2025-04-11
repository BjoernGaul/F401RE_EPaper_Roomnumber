#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"

#define COLORED     0
#define UNCOLORED   1


void paintWriteColumns(Paint paint, Epd epd);


void paintHourplan(Paint paint, Epd epd)
{
    
    //Grid
    paint.Clear(COLORED);
    for(int yPosScreen = 259 ; yPosScreen > 0; yPosScreen -= 51 )
    {
        paintVerticalLong(paint, epd, yPosScreen);

        paint.SetHeight(40);
        paint.SetWidth(8*50);
        paintHorizontalShort(paint, epd);

        epd.Display_Partial_Not_refresh(paint.GetImage(), 0, yPosScreen-paint.GetHeight()-5, 0+paint.GetWidth(), yPosScreen-5);
    }
    paintTimes(paint, epd);

}


void paintWriteColumns(Paint paint, Epd epd)
{

}
