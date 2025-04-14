#pragma once

#include "SD.h"
#include <Arduino.h>
#include <SPI.h>

File planSchoolhours;
String file = "stunde.txt";


String readSDLine(int targetLine)
{
    int currentline = 0;
    planSchoolhours =  SD.open(file);
    if (!planSchoolhours) { // Check if the file opened successfully
        Serial.println("Failed to open file");
        return "";
    }
    while(planSchoolhours.available())
    {
        String line = planSchoolhours.readStringUntil('\n');
        currentline ++;
        if(currentline == targetLine)
        {
            planSchoolhours.close(); //close after found
            return line;
        }
    } 
    planSchoolhours.close(); //ensure closing if fail
    return "";
}

String getPartByIndex(String line, int index) {
    int start = 0;
    int end = -1;
    int currentIndex = 0;

    if(line == "Leer" | line == "MO" | line == "DI" | line == "MI" | line == "DO" | line == "FR")
    {
        return line; //Exit early if line is showing empty or weekday
    }

    while (currentIndex <= index) {
        start = end + 1; // Start of the current part
        end = line.indexOf(',', start); // Find the next comma
        if (end == -1) { // If no more commas, set end to the end of the string
            end = line.length();
        }

        if (currentIndex == index) {
            return line.substring(start, end); // Return the part at the desired index
        }

        currentIndex++;
    }

    return ""; // Return an empty string if the index is out of bounds
}

