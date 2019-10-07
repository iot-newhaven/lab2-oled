/*
 * Project lab2-sensor-oled
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "SparkFunMicroOLED.h" // Include MicroOLED library
#include "math.h"
#include "adxl362.h"

#define BOARD_SYSTEM_LED                D7

#define SENSOR_SAMPLE_RATE_MS           (100)

#define SENSOR_DISPLAY_UPDATE_MS        (5000)

#define SENSOR_TEMP_LOG_SAMPLES         (100)

ADXL362 sensor;

int16_t XValue, YValue, ZValue, Temperature;

float tempAvgCelsius = 0;

float sensorTempLog[SENSOR_TEMP_LOG_SAMPLES];

bool sensorLogFirsTime = true;

int sensorLogOffset = 0;

float sensorTempAvg = 0;

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(MODE_I2C, D8, 1); // I2C declaration

void displayInit(void)
{
    oled.begin();         // Initialize the OLED
    oled.clear(PAGE);     // Clear the screen
    oled.setFontType(1);  // Set font to type 0
    oled.setCursor(0, 0); // Set cursor to top-left

    oled.print("Hello!");
    oled.display();
}

void sensorTempGetRawVal(void)
{
    // read all three axis in burst to ensure all measurements correspond to same sample time
    //sensor.readXYZTData(XValue, YValue, ZValue, Temperature);
    Temperature = sensor.readTemp();
    //Serial.println(Temperature);
}

float sensorTempGetCelsius(void)
{
    float tempCelsius = 0;

    tempCelsius = (float)Temperature * 0.065;

    return tempCelsius;
}

// Reads sample from the sensor
void sensorTempGetSample(void)
{
    int i;

    if(sensorLogFirsTime)
    {
        for(i = 0 ; i < SENSOR_TEMP_LOG_SAMPLES ; i++)
        {
            sensorTempLog[i] = 0.0;
        }

        sensorLogFirsTime = false;

        sensorLogOffset = 0;
    }

    // Get raw data values from sensor
    sensorTempGetRawVal();

    // Convert raw value to celsius
    // record sample
    sensorTempLog[sensorLogOffset] = sensorTempGetCelsius();

    sensorLogOffset++;

    if(sensorLogOffset > SENSOR_TEMP_LOG_SAMPLES)
    {
        sensorLogOffset = 0;
    }

    // Calculate running Average
    sensorTempAvg = 0;

    for (i = 0; i < SENSOR_TEMP_LOG_SAMPLES; i++)
    {
        sensorTempAvg += sensorTempLog[i];
    }

    sensorTempAvg = (sensorTempAvg/SENSOR_TEMP_LOG_SAMPLES);
}

// Displays Temperature value in degree celsius to the serial monitor
void sensorTempSerialDisplay(float tempCelsius)
{
    static char messageBuffer[255];

    memset(messageBuffer, 0, sizeof(messageBuffer));

    snprintf(messageBuffer, sizeof(messageBuffer), "Temperature %f C\n", tempCelsius);

    Serial.print(messageBuffer);
}

void sensorTempOLEDupdate(float tempCelsius)
{
    static char tempBuffer[32];

    oled.clear(PAGE);     // Clear the screen
    
    oled.setFontType(1);  // Set font to type 0
    
    oled.setCursor(0, 0); // Set cursor to top-left

    memset(tempBuffer, 0, sizeof(tempBuffer));

    snprintf(tempBuffer, sizeof(tempBuffer), "%.1f C", tempCelsius);

    oled.print(tempBuffer);
    
    oled.display();
}

// Display Average Temperature
void sensorTempDisplayAverage(void)
{
    sensorTempSerialDisplay(sensorTempAvg);

    sensorTempOLEDupdate(sensorTempAvg);
}

// Temperature sensor Init and timer objects
Timer timerSensorTempDisplay(SENSOR_DISPLAY_UPDATE_MS, sensorTempDisplayAverage);

Timer timerSensorTempSample(SENSOR_SAMPLE_RATE_MS, sensorTempGetSample);

void sensorTempInit(void)
{
    sensor.begin(SS);      // Setup SPI protocol, issue device soft reset
    sensor.beginMeasure(); // Switch ADXL362 to measure mode

    // Start Temperature sample timer
    timerSensorTempSample.start();

    // Start Temperature display timer
    timerSensorTempDisplay.start();

}

// System heartbeat loop()
void systemHeartbeat(void)
{
    static bool firstTime = true;
    static bool ledState = true;

    if (firstTime)
    {
        pinMode(BOARD_SYSTEM_LED, OUTPUT);

        firstTime = false;

        digitalWrite(BOARD_SYSTEM_LED, HIGH);
    }
    else
    {
        if (ledState)
        {
            digitalWrite(BOARD_SYSTEM_LED, LOW);

            ledState = false;
        }
        else
        {
            digitalWrite(BOARD_SYSTEM_LED, HIGH);

            ledState = true;
        }
    }

    delay(1000); // Delay 1000 ms
}

// setup() runs once, when the device is first turned on.
void setup()
{
    // Put initialization like pinMode and begin functions here.
    delay(100);

    Serial.begin(115200);
    
    Serial.println("Lab2: OLED sensor (task 2b)");

    displayInit();
    
    delay(2000);
    
    sensorTempInit();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // The core of your code will likely live here.
    systemHeartbeat();
}