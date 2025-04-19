#pragma once

#include "SD.h"
#include <Arduino.h>
#include <SPI.h>

File planSchoolhours;
String file = "stunde.txt";
char dataSchoolhoursbuffer[10000];



void readAllSDData() {
    int bufferIndex = 0; // Index to track the position in the buffer
    planSchoolhours = SD.open(file);
    Serial.println("Reading data from SD card...");

    if (!planSchoolhours) { // Check if the file opened successfully
        Serial.println("Failed to open file");
        return;
    }

    while (planSchoolhours.available()) {
        // Read a line from the SD card
        String line = planSchoolhours.readStringUntil('\n');
        // Serial.println(line); // Print the line to the Serial Monitor

        // Convert the line to a char array
        line.toCharArray((char*)&dataSchoolhoursbuffer[bufferIndex], sizeof(dataSchoolhoursbuffer) - bufferIndex);

        // Move the buffer index forward by the length of the line
        bufferIndex += line.length();

        // Add a separator (e.g., '\n' or '|') after the line
        if (bufferIndex < sizeof(dataSchoolhoursbuffer) - 1) {
            dataSchoolhoursbuffer[bufferIndex] = '|'; // Use '\n' as a separator
            bufferIndex++;
        }
        Serial.println(line); // Print the line to the Serial Monitor
        Serial.println(bufferIndex); // Print the buffer index to the Serial Monitor

        // Ensure we don't overflow the buffer
        if (bufferIndex >= sizeof(dataSchoolhoursbuffer) - 1) {
            Serial.println("Buffer overflow! Data truncated.");
            break;
        }
        delay(20); //Delay for not overwhelming SD card / SPI Bus
    }

    // Null-terminate the buffer
    dataSchoolhoursbuffer[bufferIndex] = '\0';

    planSchoolhours.close(); // Close the file
    Serial.println("Data read from SD card");
}

