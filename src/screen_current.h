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

bool getCurrentStatus(char* data, int hour, int indexDay);
void paintTemperature(Paint paint, Epd epd, float temperature);
void paintSensorDate(Paint paint, Epd epd, float sensorData, bool isTemperature);

void paintCurrentInformation(Paint paint, Epd epd, char* data, char* date, int time, int Weekday, float temperature, float humidity)
{
    // Clear the display
    paint.Clear(UNCOLORED);
    
    paintUpperLeftCorner(paint, epd, Weekday, date);

    //Title
    paint.SetHeight(260);
    paint.SetWidth(8*5);
    paint.Clear(UNCOLORED);

    if(getCurrentStatus(data, time, getCurrentDayIndex(data, Weekday)))
    {
        paint.DrawStringAt(50, 5, "Belegt", &Font24, COLORED); // Display "Occupied" if the current hour is occupied
    } else{
        paint.DrawStringAt(50, 5, "Frei", &Font24, COLORED); // Display "Free" if the current hour is free
    }

    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 0, 0+paint.GetWidth(), 0+paint.GetHeight());

    paintSensorDate(paint, epd, temperature, Tempdata); // Display the temperature on the e-paper display
    paintSensorDate(paint, epd, humidity, Humiditydata); // Display the humidity on the e-paper display


}

bool getCurrentStatus(char* data, int hour, int indexDay)
{
    bool occupied = false; // Flag to indicate if the current hour is occupied
    int indexHour = getCurrentHourIndex(data, hour, indexDay); // Get the current hour index from the data string

    if (indexHour == -1) {
        Serial.println("Error indexHour not found");
        return occupied; // Return false if the index is not found
    }else if(indexHour == 0){
        Serial.println("Ausserhalb der Schulzeit");
        return occupied; // Return false if the index is not found
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
        paint.DrawStringAt(50, 10, "Temperatur", &Font20, COLORED); // Display "Temperature" if the current hour is occupied
        epd.Display_Partial_Not_refresh(paint.GetImage(), 41, 0, 41+paint.GetWidth(), 0+paint.GetHeight());
    } else{
        paint.DrawStringAt(50, 10, "Luftfeuchtigkeit", &Font20, COLORED); // Display "Humidity" if the current hour is free
        epd.Display_Partial_Not_refresh(paint.GetImage(), 181, 0, 181+paint.GetWidth(), 0+paint.GetHeight());
    }

    // Data
    paint.SetHeight(160);
    paint.SetWidth(8*10);
    paint.Clear(UNCOLORED);
    if(isTemperature)
    {
        char tempStr[7];
        sprintf(tempStr, "%.2f C", sensorData); // Format the string with the temperature value
        Serial.println(tempStr); // Print the temperature value to the serial monitor
        paint.DrawStringAt(0, 0, tempStr, &Font24, COLORED);
    
        // Display the current information on the e-paper display
        epd.Display_Partial_Not_refresh(paint.GetImage(), 71, 220-paint.GetHeight(), 71+paint.GetWidth(), 220);
    } else{
        char tempStr[7];
        sprintf(tempStr, "%.2f %", sensorData); // Format the string with the temperature value
        Serial.println(tempStr); // Print the temperature value to the serial monitor
        paint.DrawStringAt(0, 0, tempStr, &Font24, COLORED);
    
        // Display the current information on the e-paper display
        epd.Display_Partial_Not_refresh(paint.GetImage(), 241, 220-paint.GetHeight(), 241+paint.GetWidth(), 220);
    }

}