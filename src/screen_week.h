#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"

#define COLORED     0
#define UNCOLORED   1


void paintWriteRows(Paint paint, Epd epd, char* data);


void paintHourplan(Paint paint, Epd epd, char* data)
{
    
    //Grid
    paint.Clear(UNCOLORED);
    for(int yPosScreen = 259 ; yPosScreen > 0; yPosScreen -= 51 )
    {
        paintVerticalLong(paint, epd, yPosScreen);
        paint.Clear(UNCOLORED);
    }
    paint.Clear(UNCOLORED);
    paintTimes(paint, epd); // Call the function to paint the times
    paint.Clear(UNCOLORED);
    paintWriteRows(paint, epd, data); // Call the function to write the rows of data

}


void paintWriteRows(Paint paint, Epd epd, char* data) {
    // Function to write the rows of data to the screen

    //get the index of the day in data
    int dayIndices[5] = {-1, -1, -1, -1, -1}; // Array to store indices for MO, DI, MI, DO, FR
    const char* days[] = {"MO", "DI", "MI", "DO", "FR"}; // Array of day markers

    // Iterate through each day
    for (int day = 0; day < 5; day++) {
        const char* compareDay = days[day]; // Get the current day marker
        for (int i = 0; i < strlen(data); i++) {
            if (data[i] == compareDay[0] && data[i + 1] == compareDay[1] && data[i + 2] == '|') {
                dayIndices[day] = i + 3; // Store the index after the day marker
                break;
            }
        }
        if (dayIndices[day] == -1) {
            // Serial.print("Day ");
            // Serial.print(compareDay);
            // Serial.println(" not found in data");
        }
    }

    for (int day = 0; day < 5; day++) {
        paint.SetWidth(32);
        paint.SetHeight(48);
        paint.Clear(UNCOLORED);

        paint.DrawStringAt(2, 4, days[day], &Font24, COLORED); // Draw the day marker
        paint.DrawHorizontalLine(0, 0, 48, COLORED); // Draw a horizontal line for the current day
        paint.DrawHorizontalLine(0, 31, 48, COLORED); // Draw a horizontal line for the current day
        epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 258 - (51 * day) - 48, 0 + 32, 258 - (51 * day));
    }

    // Iterate through each hour and write the data to the screen
    for (int hour = 1; hour < 12; hour++) {

        // Paint the data for the current day
        for (int day = 0; day < 5; day++) {
            int indexHour = dayIndices[day]; // Get the index for the current day
            // Serial.println("Paint data");
            paint.SetWidth(32);
            paint.SetHeight(48);
            paint.Clear(UNCOLORED);

            // Find the length of the current hour data between '|' separators
            int entryLength = 0;
            while (data[indexHour + entryLength] != '|' && data[indexHour + entryLength] != '\0') {
                entryLength++;
            }

            // Extract the current entry
            char entry[entryLength + 1];
            strncpy(entry, &data[indexHour], entryLength);
            entry[entryLength] = '\0';
            // Serial.println(entry); // Print the entry to the Serial Monitor

            if (strcmp(entry, "Leer\0") == 0) {
                // Serial.println("Leer\0");
                dayIndices[day] += entryLength + 1; // Skip this entry and move to the next
                paint.DrawHorizontalLine(0, 0, 48, COLORED); // Draw a horizontal line for the current hour
                paint.DrawHorizontalLine(0, 31, 48, COLORED);
                epd.Display_Partial_Not_refresh(paint.GetImage(), (hour-1)*32+32, 258-(51*day)-48, (hour-1)*32+32 + 32, 258-(51*day));
                continue; // Skip to the next entry
            }

            // Parse the hour, subject, and room from the entry
            char* subject = strtok(entry, ",");
            char* schoolclass = strtok(nullptr, ",");
            char* teacher = strtok(nullptr, ",");

            if (subject && schoolclass && teacher) {
                // Paint the hour, subject, and room
                paint.DrawStringAt(2, 4, subject, &Font8, COLORED);
                paint.DrawStringAt(2, 13, schoolclass, &Font8, COLORED);
                paint.DrawStringAt(2, 22, teacher, &Font8, COLORED);
            }
            paint.DrawHorizontalLine(0, 0, 48, COLORED); // Draw a horizontal line for the current hour
            paint.DrawHorizontalLine(0, 31, 48, COLORED);
            epd.Display_Partial_Not_refresh(paint.GetImage(), (hour-1)*32+32, 258-(51*day)-48, (hour-1)*32+32+32, 258-(51*day));

            // Move to the next part in data
            dayIndices[day] += entryLength + 1; // +1 to skip the '|' separator
            // Serial.println(hour);
        }
        
    }
}


