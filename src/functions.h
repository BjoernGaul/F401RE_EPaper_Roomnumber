#pragma once

#include <Arduino.h>

int getCurrentDayIndex(char* data, int indWeekday)
{
    char compareDay[3] = "MO"; //if the day is not found, it will be set to Monday

    if      (indWeekday == 1){compareDay[0] = 'M'; compareDay[1] = 'O'; //get the right Day to compare for search in data
    }else if(indWeekday == 2){compareDay[0] = 'D'; compareDay[1] = 'I';//DI
    }else if(indWeekday == 3){compareDay[0] = 'M'; compareDay[1] = 'I';//MI
    }else if(indWeekday == 4){compareDay[0] = 'D'; compareDay[1] = 'O';//DO
    }else if(indWeekday == 5){compareDay[0] = 'F'; compareDay[1] = 'R';//FR
    }
    

    //look for index of day in data
    int indexDay = -1;
    for(int i = 0; i < strlen(data); i++)
    {
        if(data[i] == compareDay[0] && data[i+1] == compareDay[1] && data[i+2] == '|')
        {
            indexDay = i+3; //to get to the first character of the first hour
            // Serial.println("indexDay set");
            // Serial.println(indexDay);
            return indexDay;
        }
    }
    return indexDay; // Return -1 if the day is not found
}

int getCurrentHourIndex(char* data, float time, int indexDay)
{
    Serial.print(time);
    Serial.println("  time");

    // Get school hour from time
    int schoolHour = 0;
    if (8.0f < time && time < 8.75f) // 8:00 - 8:42
    {
        schoolHour = 1;
    }
    else if (8.75f < time && time < 9.5f) // 8:42 - 9:25
    {
        schoolHour = 2;
    }
    else if (9.75f < time && time < 10.5f) // 9:45 - 10:30
    {
        schoolHour = 3;
    }
    else if (10.5f < time && time < 11.25f) // 10:30 - 11:15
    {
        schoolHour = 4;
    }
    else if (11.5f < time && time < 12.25f) // 11:30 - 12:15
    {
        schoolHour = 5;
    }
    else if (12.25f < time && time < 13.0f) // 12:30 - 13:15
    {
        schoolHour = 6;
    }
    else if (13.0f < time && time < 13.75f) // 13:15 - 14:00
    {
        schoolHour = 7;
    }
    else if (13.75f < time && time < 14.5f) // 14:00 - 14:45
    {
        schoolHour = 8;
    }
    else if (14.75f < time && time < 15.5f) // 14:45 - 15:30
    {
        schoolHour = 9;
    }
    else if (15.5f < time && time < 16.25f) // 15:30 - 16:15
    {
        schoolHour = 10;
    }
    else if (16.25f < time && time < 17.0f) // 16:15 - 17:00
    {
        schoolHour = 11;
    }
    else
    { // 17:00 - 8:00
        schoolHour = 0;
        return -1;
    }


    // Look for index of hour in data
    int indexHour = indexDay; // Start from the index of the current day

    for (int i = 1; i < schoolHour; i++)
    {
        int entryLength = 0;
        while (data[indexHour + entryLength] != '|' && data[indexHour + entryLength] != '\0')
        {
            entryLength++;
        }
        indexHour += entryLength + 1; // Skip this entry and move to the next
    }
    return indexHour;
}