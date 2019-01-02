/*
  CarSensor.h - Library for reading cheap car parking ultra sonic module with 4 sensors.
  Created by Eric Winck, November 16, 2018.
  Released by Alice Wonders
*/


#ifndef CarSensor_h
#define CarSensor_h

#include "Arduino.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TOTAL_SENSORS 4             // number of single sensors per module. 4 is default
#define FIRST_PULSE_MAX 3600        // microseconds: max values for the first sync pulses
#define FIRST_PULSE_MIN 3000        // microseconds: min values for the first sync pulses
#define MAX_BITS 50                 // pulse train bit counter
#define BIG_PULSE_MIN 10000         // microseconds: min value between entirepulses from module
#define NUMBER_PULSES_CALIBRATION 8 // number of the first calibration bits sent by the module. Every module has diferent times between pulses.

////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CarSensor {
    public:
        CarSensor(byte pin);
        void begin();
        void printDebug();
        void read();
        int getSensor(byte sensorN);
        int getGroupDistance(byte groupN);

    private:

        void calibrate();
        byte _pin;
        byte state = 1;
        int counterBits = 0;
        byte arrayBits[MAX_BITS];
        int tempSensors[TOTAL_SENSORS];
        int distanceSensors[TOTAL_SENSORS];
        int distanceStation[TOTAL_SENSORS/2];
        int proximitySensors[TOTAL_SENSORS];
        int connectedSensors[TOTAL_SENSORS];
        boolean flagCalibrated = false;
        boolean flagSensorFound = true;

        // tabela de codigos da distancia que traduzem os bits enviados pelo sistema
        int scale[100] = {0,64,98,18,82,50,114,10,74,42,106,26,90,58,122,6,70,38,102,22,86,54,118,14,78,46,110,30,94,62,126,1,65,33,97,17,85,53,81,49,113,9,73,41,105,25,89,57,121,5,69,37,101,21};

        int pulseTotalTime = 0;
        int pulseTime = 0;
        unsigned int pulseValue;
        unsigned long pm = 0;
        unsigned long cm;
        byte countMedia;
};

#endif
