#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"
#include "functions.h"

void paintTeam(Epd epd, Paint paint, int indweekday, char* date)
{
    paintUpperLeftCorner(paint, epd, indweekday, date);

    //Title
    paint.SetHeight(240);
    paint.SetWidth(8*5);
    paint.Clear(UNCOLORED);

    paint.DrawStringAt(50, 5, "Made by", &Font24, COLORED);

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 0, 0+paint.GetWidth(), 0+paint.GetHeight());
    //Paint Names

    paint.SetHeight(320);
    paint.SetWidth(8*7);
    paint.Clear(UNCOLORED);
    paint.DrawHorizontalLine(0, 0, 320, COLORED); 
    paint.DrawStringAt(50, 5, "Bjoern", &Font24, COLORED); 
    paint.DrawStringAt(50, 35, "Gaul", &Font24, COLORED);
    epd.Display_Partial_Not_refresh(paint.GetImage(), 50, 0, 50+paint.GetWidth(), 0+paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(50, 10, "&", &Font24, COLORED);
    epd.Display_Partial_Not_refresh(paint.GetImage(), 110, 0, 106+paint.GetWidth(), 0+paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(50, 5, "Sebastian", &Font24, COLORED); 
    paint.DrawStringAt(50, 35, "Ittner", &Font24, COLORED);
    epd.Display_Partial_Not_refresh(paint.GetImage(), 181, 0, 181+paint.GetWidth(), 0+paint.GetHeight());

}