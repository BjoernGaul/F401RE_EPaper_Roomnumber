#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"
#include "sd_reader.h"
#include "functions.h"

#define COLORED     0
#define UNCOLORED   1




void paintHourplanDay(Paint paint, Epd epd, char* data, int indWeekday, char* date, int indexDay)
{
    int partsInd = 1;

    //Title
    paint.SetWidth(24);
    paint.SetHeight(248);
    paint.Clear(UNCOLORED);

    if(indWeekday == 1){paint.DrawStringAt(50, 0, "Montag", &Font20, COLORED);
    }else if(indWeekday == 2){paint.DrawStringAt(50, 0, "Dienstag", &Font20, COLORED);
    }else if(indWeekday == 3){paint.DrawStringAt(50, 0, "Mittwoch", &Font20, COLORED);
    }else if(indWeekday == 4){paint.DrawStringAt(50, 0, "Donnerstag", &Font20, COLORED);
    }else if(indWeekday == 5){paint.DrawStringAt(50, 0, "Freitag", &Font20, COLORED);
    }
    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 0, 0+paint.GetWidth(), 0+paint.GetHeight());
    // Serial.println("Title set");

    int indexHour = indexDay;

    //Paint data
    for (int i = 1; i < 12; i++) {
        // Serial.println("Paint data");
        paint.SetWidth(32);
        paint.SetHeight(248);
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
            indexHour += entryLength + 1; // Skip this entry and move to the next
            // Draw a horizontal line for the current hour
            paint.DrawHorizontalLine(0, 0, 248, COLORED); // Draw a horizontal line for the current hour
            paint.DrawHorizontalLine(0, 31, 248, COLORED); // Draw a horizontal line for the current hour
            epd.Display_Partial_Not_refresh(paint.GetImage(), (i-1)*32+32, 0, (i-1)*32+32 + 32, 248);
            continue;
        }

        // Parse the hour, subject, and room from the entry
        char *subject = strtok(entry, ",");
        char *schoolclass = strtok(nullptr, ",");
        char *teacher = strtok(nullptr, ",");

        if (subject && schoolclass && teacher) {
            // Paint the hour, subject, and room
            paint.DrawStringAt(10, 5, subject, &Font16, COLORED);
            paint.DrawStringAt(50, 5, schoolclass, &Font16, COLORED);
            paint.DrawStringAt(150, 5, teacher, &Font16, COLORED);
        }

        // Draw a horizontal line for the current hour
        paint.DrawHorizontalLine(0, 0, 248, COLORED); // Draw a horizontal line for the current hour
        paint.DrawHorizontalLine(0, 31, 248, COLORED); // Draw a horizontal line for the current hour
        // Move to the next part in data
        indexHour += entryLength + 1; // +1 to skip the '|' separator
        // Serial.println(i);
        epd.Display_Partial_Not_refresh(paint.GetImage(), (i-1)*32+32, 0, (i-1)*32+32 + 32, 248);
    }

    paintTimes(paint, epd, date, indWeekday);
}



