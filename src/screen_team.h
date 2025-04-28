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
    paint.SetHeight(280);
    paint.SetWidth(8*5);
    paint.Clear(UNCOLORED);

    paint.DrawStringAt(50, 0, "Made by", &Font20, COLORED);
    paint.DrawHorizontalLine(0, 32, 280, COLORED); // Draw a horizontal line for the current hour

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 40, 0+paint.GetWidth(), 0+paint.GetHeight());

    //Paint Names

    paint.SetHeight(320);
    paint.SetWidth(8*7);
    paint.Clear(UNCOLORED);
    paint.DrawHorizontalLine(0, 0, 320, COLORED); 
    paint.DrawStringAt(50, 0, "Bjoern Gaul", &Font24, COLORED); 
    epd.Display_Partial_Not_refresh(paint.GetImage(), 41, 0, 41+paint.GetWidth(), 0+paint.GetHeight());

    
    paint.SetHeight(320);
    paint.SetWidth(8*7);
    paint.Clear(UNCOLORED);
    paint.DrawHorizontalLine(0, 0, 320, COLORED); 
    paint.DrawStringAt(50, 0, "Sebastian Ittner", &Font24, COLORED); 
    epd.Display_Partial_Not_refresh(paint.GetImage(), 181, 0, 181+paint.GetWidth(), 0+paint.GetHeight());

}