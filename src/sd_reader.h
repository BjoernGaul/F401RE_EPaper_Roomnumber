#include "SD.h"
#include <Arduino.h>
#include <SPI.h>

File planSchoolhours;

void initSD()
{

    if(SD.begin(D5))
    {
        Serial.println("SD-init failed");
        return;
    }
}

String readSDLine(int targetLine)
{
    int currentline = 0;
    planSchoolhours =  SD.open("Stundenplan.txt");
    if (!planSchoolhours) { // Check if the file opened successfully
        Serial.println("Failed to open Stundenplan.txt");
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

