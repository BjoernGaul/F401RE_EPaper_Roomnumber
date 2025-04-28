#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"
#include "functions.h"

#define COLORED     0
#define UNCOLORED   1
#define Tempdata true
#define Humiditydata false

void paintTemperature(Paint paint, Epd epd, float temperature);

void paintCurrentInformation(Paint paint, Epd epd, char* data, char* date, int time, int indWeekday, int temperature)
{
    // Clear the display
    paint.Clear(UNCOLORED);
    
    paint.SetHeight(40);
    paint.SetWidth(8*4);
    paint.Clear(UNCOLORED);

    //Day
    if(indWeekday = 1){paint.DrawStringAt(0, 0, "Montag", &Font8, COLORED);
    }else if(indWeekday = 2){paint.DrawStringAt(0, 0, "Dienstag", &Font8, COLORED);
    }else if(indWeekday = 3){paint.DrawStringAt(0, 0, "Mittwoch", &Font8, COLORED);
    }else if(indWeekday = 4){paint.DrawStringAt(0, 0, "Donnerstag", &Font8, COLORED);
    }else if(indWeekday = 5){paint.DrawStringAt(0, 0, "Freitag", &Font8, COLORED);
    }else if(indWeekday = 6){paint.DrawStringAt(0, 0, "Samstag", &Font8, COLORED);
    }else if(indWeekday = 0){paint.DrawStringAt(0, 0, "Sonntag", &Font8, COLORED);
    }

    //Date
    paint.DrawStringAt(0, 15, date, &Font8, COLORED);

    paint.DrawHorizontalLine(0, 32, 40, COLORED);
    paint.DrawVerticalLine(39, 0, 40, COLORED); // Draw a vertical line to separate the date and time from Title
    
    // Display the current information on the e-paper display
    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 0, 0+paint.GetWidth(), 0+paint.GetHeight());

    //Title
    paint.SetHeight(280);
    paint.SetWidth(8*5);
    paint.Clear(UNCOLORED);

    if(getCurrentStatus(data, time, indWeekday))
    {
        paint.DrawStringAt(50, 0, "Belegt", &Font20, COLORED); // Display "Occupied" if the current hour is occupied
    } else{
        paint.DrawStringAt(50, 0, "Frei", &Font20, COLORED); // Display "Free" if the current hour is free
    }

    paint.DrawHorizontalLine(0, 32, 280, COLORED); // Draw a horizontal line for the current hour

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 40, 0+paint.GetWidth(), 0+paint.GetHeight());

    paintSensorDate(paint, epd, temperature); // Display the temperature on the e-paper display


}

bool getCurrentStatus(char* data, int hour, int indexDay)
{
    bool occupied = false; // Flag to indicate if the current hour is occupied
    int indexHour = getCurrentHourIndex(data, hour, indexDay); // Get the current hour index from the data string

    if (indexHour == -1) {
        Serial.println("Error indexHour not found");
        return;
    }else if(indexHour == 0){
        Serial.println("Ausserhalb der Schulzeit");
        return;
    }

    int entryLength = 0;
    // Find the length of the current hour data between '|' separators
    while (data[indexHour + entryLength] != '|' && data[indexHour + entryLength] != '\0') {
        entryLength++;
    }

    if(entryLength > 6)
    {
        occupied = true; // Limit the length to 6 characters
    }
    return occupied; // Return the occupied status
}

void paintSensorDate(Paint paint, Epd epd, float sensorData, bool isTemperature)
{

    paint.SetHeight(320);
    paint.SetWidth(8*7);
    paint.Clear(UNCOLORED);
    paint.DrawHorizontalLine(0, 0, 320, COLORED); 

    if(isTemperature)
    {
        paint.DrawStringAt(50, 0, "Temperatur", &Font20, COLORED); // Display "Temperature" if the current hour is occupied
        epd.Display_Partial_Not_refresh(paint.GetImage(), 41, 0, 41+paint.GetWidth(), 0+paint.GetHeight());
    } else{
        paint.DrawStringAt(50, 0, "Luftfeuchtigkeit", &Font20, COLORED); // Display "Humidity" if the current hour is free
        epd.Display_Partial_Not_refresh(paint.GetImage(), 181, 0, 181+paint.GetWidth(), 0+paint.GetHeight());
    }




    // Data
    paint.SetHeight(160);
    paint.SetWidth(8*15);
    paint.Clear(UNCOLORED);
    if(isTemperature)
    {
        char tempStr[7];
        sprintf(tempStr, "%.2f C", sensorData); // Format the string with the temperature value
        paint.DrawStringAt(0, 0, tempStr, &Font24, COLORED);
    
        // Display the current information on the e-paper display
        epd.Display_Partial_Not_refresh(paint.GetImage(), 71, 80, 71+paint.GetWidth(), 0+paint.GetHeight());
    } else{
        char tempStr[7];
        sprintf(tempStr, "%.2f %", sensorData); // Format the string with the temperature value
        paint.DrawStringAt(0, 0, tempStr, &Font24, COLORED);
    
        // Display the current information on the e-paper display
        epd.Display_Partial_Not_refresh(paint.GetImage(), 241, 80, 241+paint.GetWidth(), 0+paint.GetHeight());
    }

}