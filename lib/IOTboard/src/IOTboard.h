// IOT Display class

#ifndef IOTBOARD_H
#define IOTBOARD_H

#include "application.h"
// Include MicroOLED library
#include "SparkFunMicroOLED.h" 
#include "math.h"

#include "SparkFunTMP102.h"

#define BOARD_SYSTEM_LED      D7
#define BOARD_HEARTBEAT_MS    (1000)

/* code */
class IOTboard
{
public:
   IOTboard();

   void start();

   void printToDisplay(const char* text);

   float getTempF();

   void serialLog(const char *fmt, ...);

private:
   char serial_out_buffer[255];
};

#endif