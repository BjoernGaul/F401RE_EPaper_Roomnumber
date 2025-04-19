#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "epdpaint.h"
#include "paint_templates.h"
#include "sd_reader.h"

#define COLORED     0
#define UNCOLORED   1




void paintHourplanDay(Paint paint, Epd epd, char* data, int indWeekday)
{
    int partsInd = 1;
    char compareDay[3] = "MO";

    if(indWeekday = 1){; //get the right Day to compare for search in data
    }else if(indWeekday = 2){compareDay[0] = 'D'; compareDay[1] = 'I'; compareDay[2] = '\0' ;//DI
    }else if(indWeekday = 3){compareDay[0] = 'M'; compareDay[1] = 'I'; compareDay[2] = '\0' ;//MI
    }else if(indWeekday = 4){compareDay[0] = 'D'; compareDay[1] = 'O'; compareDay[2] = '\0' ;//DO
    }else if(indWeekday = 5){compareDay[0] = 'F'; compareDay[1] = 'R'; compareDay[2] = '\0' ;//FR
    }
    
    Serial.println("compareDay set");

    //look for index of day in data
    int indexDay = -1;
    for(int i = 0; i < sizeof(data); i++)
    {
        if(data[i] == compareDay[0] && data[i+1] == compareDay[1] && data[i+2] == '|')
        {
            indexDay = i+3; //to get to the first character of the first hour
            break;
        }
    }


    //Grid
    paint.Clear(UNCOLORED);
    for(int yPosScreen = 259 ; yPosScreen > 0; yPosScreen -= 51 )
    {
        //Basic Grid
        paint.SetHeight(40);
        paint.SetWidth(8*50);
        paintHorizontalShort(paint, epd);

        epd.Display_Partial_Not_refresh(paint.GetImage(), 0, yPosScreen-paint.GetHeight()-5, 0+paint.GetWidth(), yPosScreen-5);
    }

    Serial.println("Grid set");

    //Title
    paint.Clear(UNCOLORED);
    paint.SetWidth(24);
    paint.SetHeight(200);

    if(indWeekday = 1){paint.DrawStringAt(10, 0, "Montag", &Font20, COLORED);
    }else if(indWeekday = 2){paint.DrawStringAt(10, 0, "Dienstag", &Font20, COLORED);
    }else if(indWeekday = 3){paint.DrawStringAt(10, 0, "Mittwoch", &Font20, COLORED);
    }else if(indWeekday = 4){paint.DrawStringAt(10, 0, "Donnerstag", &Font20, COLORED);
    }else if(indWeekday = 5){paint.DrawStringAt(10, 0, "Freitag", &Font20, COLORED);
    }
    epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 58, 0+paint.GetWidth(), 258);
    Serial.println("Title set");

    int indexHour = indexDay;

    //Paint data
    for (int i = 1; i < 12; i++) {
        Serial.println("Paint data");
        paint.Clear(UNCOLORED);
        paint.SetWidth(16);
        paint.SetHeight(200);
        // Find the length of the current hour data between '|' separators
        int entryLength = 0;
        while (data[indexHour + entryLength] != '|' && data[indexHour + entryLength] != '\0') {
            entryLength++;
        }

        // Extract the current entry
        char entry[entryLength + 1];
        strncpy(entry, &data[indexHour], entryLength);
        entry[entryLength] = '\0';

        if (strcmp(entry, "Leer") == 0) {
            Serial.println("Leer\0");
            indexHour += entryLength + 1; // Skip this entry and move to the next
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

        // Move to the next part in data
        indexHour += entryLength + 1; // +1 to skip the '|' separator
        Serial.println(i);
        epd.Display_Partial_Not_refresh(paint.GetImage(), i*34, 58, i*34 + 16, 258);
    }

    paintTimes(paint, epd);
}



