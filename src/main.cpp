#include <Arduino.h>
#include <SPI.h>
#include "epd4in2_V2.h"
#include "imagedata.h"
#include "epdpaint.h"
//  #include "paint_templates.h"
#include "screen_week.h"
#include "screen_day.h"
#include "SD.h"
#include "sd_reader.h"
#include "Wire.h"
#include "rtc_datetime.h"
#include "Adafruit_BME280.h"
#include "screen_current.h"
#include "screen_team.h"

Epd epd;
unsigned char image[20000];
Paint paint(image, 8*50, 1);    //width should be the multiple of 8 

uint32_t SerialClock = PB8;
uint32_t SerialData = PB9;
TwoWire MyWire(SerialData, SerialClock); // Create a TwoWire object with custom SDA and SCL pins

RTC_DS3231 rtc; // Create an RTC object
int weekday = 0; // Variable to store the current day of the week
char Date[15]; // Buffer to store the date string
char Time[10]; // Buffer to store the time string
float digitTime = 0; // Variable to store the decimal time
int indexDay = 0; // Variable to store the index of the current day in the data string
int dayCheckTimer = 0; // Timer variable to check the day

Adafruit_BME280 bme; // use I2C interface
float temperature, humidity; // Variables to store sensor data

#define TIMER_INTERVAL_MICROSEC (5 * 60 * 1000000) // 5 minutes in microseconds
volatile bool fiveMinuteFlag = false; // Flag to indicate the timer event

void onTimerInterrupt() {
    fiveMinuteFlag = true; // Set the flag to true every 5 minutes
}

#define COLORED     0
#define UNCOLORED   1

#define sdcsPin PA9
#define ButtonPin PA8



bool ButtonPressed = false; // Flag to indicate button press state
int statePaper = 0; // State variable to track the current state of the paper

volatile unsigned long lastInterruptTime = 0;
uint64_t lastMillisDispChange = 0; // Variable to store the last time the button was pressed




void setup() {
    pinMode(LED_BUILTIN, OUTPUT); // Set the LED pin as output
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(1000); // Wait for the serial connection to establish
    Serial.println("Starting");

    Serial.println("Scanning...");

    Wire.begin(SerialData, SerialClock); // Initialize the I2C bus with custom SDA and SCL pins

    for (uint8_t address = 1; address < 127; address++) { // I2C addresses range from 0x01 to 0x7F
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0"); // Add leading zero for single-digit addresses
            Serial.println(address, HEX);
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    Serial.println("Scan complete.");
    delay(50);

    // Initialize the hardware timer
    HardwareTimer *MyTimer = new HardwareTimer(TIM2); // Use TIM2 for the timer
    MyTimer->setOverflow(TIMER_INTERVAL_MICROSEC, MICROSEC_FORMAT); // Set overflow to 5 minutes
    MyTimer->attachInterrupt(onTimerInterrupt); // Attach the interrupt handler
    MyTimer->resume(); // Start the timer

    bme.begin(0x76);

    delay(1000); // Wait for the sensor to stabilize

    Serial.println("Initializing RTC...");
    rtc.begin(); // Initialize the RTC
    delay(1000); // Wait for the sensor to stabilize
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set the RTC to the current date and time
    // delay(1000); // Wait for the sensor to stabilize

    weekday = getDayOfWeek(rtc); // Get the current day of the week
    getDate(rtc, Date); // Get the current date
    getTime(rtc, Time, &digitTime); // Get the current time

    temperature = bme.readTemperature(); // Read temperature
    humidity = bme.readHumidity(); // Read humidity

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    pinMode(ButtonPin, INPUT_PULLUP); // Set the button pin as input with pull-up resistor
    attachInterrupt(digitalPinToInterrupt(ButtonPin), []() {
        unsigned long interruptTime = millis();
        if (interruptTime - lastInterruptTime > 200) { // 200ms debounce time
            ButtonPressed = true;
            Serial.println("Button pressed!");
        }
        lastInterruptTime = interruptTime;
    }, FALLING); // Trigger on falling edge (button press)


    delay(500);

    if (!initSDCard(3, sdcsPin)) {
      Serial.println("SD card initialization failed. Halting...");
      while (true){
        digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
        delay(500); // Wait for 500 milliseconds
        digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
        delay(500); // Wait for 500 milliseconds
      }; // Halt execution
    }
    delay(500);

    readAllSDData();

    Serial.println("SD-init successfull");
    delay(100);


    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    //Serial.print(UNCOLORED);
    /* This clears the SRAM of the e-paper display */
    epd.Clear();

    paint.SetRotate(3);

     paintHourplan(paint, epd, dataSchoolhoursbuffer, Date, weekday);
    //paintCurrentInformation(paint, epd, dataSchoolhoursbuffer, Date, digitTime, weekday, temperature, humidity);

    epd.TurnOnDisplay_Partial(); // Turn on the display with fast refresh

    epd.Init();
    epd.Sleep(); // Put the display to sleep to save power
    lastMillisDispChange = millis(); // Initialize the lastMillisDispChange variable to the current time
}

void loop() {
    if(fiveMinuteFlag) {
        fiveMinuteFlag = false; // Reset the flag
        dayCheckTimer++; // Increment the timer variable
        if (dayCheckTimer >= 6) { // Check if 30 minutes have passed
            weekday = getDayOfWeek(rtc); // Get the current day of the week
            getDate(rtc, Date); // Get the current date
            indexDay = getCurrentDayIndex(dataSchoolhoursbuffer, weekday); // Get the index of the current day in the data string
            dayCheckTimer = 0; // Reset the timer variable
        }
        getTime(rtc, Time, &digitTime); // Get the current time
        temperature = bme.readTemperature(); // Read temperature
        humidity = bme.readHumidity(); // Read humidity
        Serial.print("Temperature: ");
        Serial.print(bme.readTemperature());
        Serial.print(" °C, Humidity: ");
        Serial.print(bme.readHumidity());
        Serial.println(" %");
    }

    if (ButtonPressed || (millis() - lastMillisDispChange) > (30*1000)) { // Check if the button was pressed or if 30 seconds have passed || (millis() - lastMillisDispChange) > (30*1000)
        if(statePaper >= 3){
            statePaper = 0; // Reset to the first state if it exceeds the number of states
        } else {
            statePaper++; // Increment the state variable
        }
        switch (statePaper) 
        {
            case 0:
                Serial.println("State 0: Full Hourplan");
                epd.Init();
                epd.Clear();
                paintHourplan(paint, epd, 
                            dataSchoolhoursbuffer, 
                            Date, weekday);
                epd.TurnOnDisplay_Partial();
                epd.Sleep(); 
            break;
        
            case 1:
                Serial.println("State 1: Weekday Hourplan");
                epd.Init();
                epd.Clear();
                paintHourplanDay(paint, epd, 
                                dataSchoolhoursbuffer, 
                                weekday, Date, indexDay);
                epd.TurnOnDisplay_Partial();
                epd.Sleep();     
            break;

            case 2:
                Serial.println("State 2: Current Information");
                epd.Init();
                epd.Clear();
                paintCurrentInformation(paint, epd, 
                                        dataSchoolhoursbuffer, 
                                        Date, digitTime, weekday, temperature, humidity);
                epd.TurnOnDisplay_Partial();
                epd.Sleep(); 
            break;

            case 3:
                    Serial.println("State 3: Team Information");
                    epd.Init();
                    epd.Clear();
                    paintTeam(epd, paint, weekday, Date);
                    epd.TurnOnDisplay_Partial();
                    epd.Sleep(); 

            break;

            default:
            break;
        }
        lastMillisDispChange = millis(); 
        ButtonPressed = false;
    }
}
