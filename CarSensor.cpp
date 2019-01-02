#include "Arduino.h"
#include "CarSensor.h"

boolean flagCalibrated = false;
int pulseTotalTime = 0;
int pulseTime = 0;
unsigned int pulseValue;
byte countMedia;

CarSensor::CarSensor(byte pin) {
  _pin = pin;
}

void CarSensor::begin() {
    pinMode(_pin, INPUT);
    Serial.println("");
    Serial.print("Start calibrating module pin: ");
    Serial.println(_pin);
    flagCalibrated = false;
    calibrate();
}


void CarSensor::calibrate() {
    boolean flagCalibrationOK = true;

    while((pulseValue < BIG_PULSE_MIN)) {
        pulseValue = pulseIn(_pin,LOW);
        //Serial.println(pulseValue);
        if(pulseValue == 0) {
            flagCalibrationOK = false;
            Serial.print("Pin: ");
            Serial.print(_pin);
            Serial.println(" - Calibration failed. Module not found.");
            flagSensorFound = false;
            flagCalibrated = false;
            break;
        }
    }

    if(flagCalibrationOK) {
        for(byte i=0;i<NUMBER_PULSES_CALIBRATION;i++) {
            pulseValue = pulseIn(_pin,LOW);
            pulseTotalTime += pulseValue;
        }

        pulseTime = ((pulseTotalTime/NUMBER_PULSES_CALIBRATION)*2);

        if(pulseValue == 0 || pulseTime > BIG_PULSE_MIN) {
            Serial.print("Pin: ");
            Serial.print(_pin);
            Serial.println(" - Calibration failed. Pulses not found.");
            flagCalibrated = false;
        } else {
            Serial.print("Pin: ");
            Serial.print(_pin);
            Serial.print(" - Calibration done: ");
            Serial.print(pulseTime);
            Serial.println(" micros.");
            Serial.println("");
            flagSensorFound = true;
            flagCalibrated = true;
        }

    }
}

void CarSensor::read() {


            if(flagCalibrated) {

                //find pulse train intro
                while(pulseValue < FIRST_PULSE_MIN || pulseValue > FIRST_PULSE_MAX) {
                    pulseValue = pulseIn(_pin,LOW);
                    if(pulseValue==0){
                        flagSensorFound = false;
                        break;
                    } else {
                        flagSensorFound = true;
                    }
                }


                for(byte i=0;i<MAX_BITS;i++) {
                    pulseValue = digitalRead(_pin);
                    arrayBits[i] = pulseValue;
                    delayMicroseconds(pulseTime);
                }

                arrayBits[0] = 0;
                pulseTotalTime = 0;

                // process bits from pulse train into the correct indexes
                for(int i=0;i<=7;i++) {
                    if(arrayBits[i]==1) {
                        bitSet(tempSensors[0], 7-i);
                    } else {
                        bitClear(tempSensors[0], 7-i);
                    }

                    if(arrayBits[i+16]==1) {
                        bitSet(tempSensors[1], 7-i);
                    } else {
                        bitClear(tempSensors[1], 7-i);
                    }

                    if(arrayBits[i+8]==1) {
                        bitSet(tempSensors[2], 7-i);
                    } else {
                        bitClear(tempSensors[2], 7-i);
                    }

                    if(arrayBits[i+24]==1) {
                        bitSet(tempSensors[3], 7-i);
                    } else {
                        bitClear(tempSensors[3], 7-i);
                    }
                }

                proximitySensors[0] = arrayBits[41]; // proximity bit
                proximitySensors[1] = arrayBits[43]; // proximity bit
                proximitySensors[2] = arrayBits[45]; // proximity bit
                proximitySensors[3] = arrayBits[47]; // proximity bit

                connectedSensors[0] = arrayBits[42]; // sensor connected bit
                connectedSensors[1] = arrayBits[44]; // sensor connected bit
                connectedSensors[2] = arrayBits[46]; // sensor connected bit
                connectedSensors[3] = arrayBits[48]; // sensor connected bit

                //convert data from pulse train into units using the SCALE array reference

                for(int s=0;s<TOTAL_SENSORS;s++) {

                    int escalaIndex = -1;

                    for(int i=0;i<(sizeof(scale)/sizeof(int));i++) { // find received value inside the SCALE array
                        if(tempSensors[s] == scale[i]) {
                            escalaIndex = i;
                        }
                    }

                    if(escalaIndex != -1) { // did find the value inside SCALE array
                        if(proximitySensors[s] == 0 && tempSensors[s] != 0) { // if the distance is ABOVE 30cm
                            distanceSensors[s] = escalaIndex+1;
                        } else if (proximitySensors[s] == 0 && tempSensors[s] == 0) {
                            distanceSensors[s] = 0;
                        } else if (proximitySensors[s] == 1 && tempSensors[s] == 71) { // error
                            distanceSensors[s] = 0;
                        } else if (proximitySensors[s] == 1 && tempSensors[s] == 0) {  // error
                            distanceSensors[s] = 0;
                        } else {  // if the distance is BELOW 30cm
                            distanceSensors[s] = 1;
                        }
                    } else {  // did NOT find the value inside SCALE array
                        distanceSensors[s] = 0; // assume the max distance
                    }

                }

            } else { // module not calibrated
                calibrate();
            }


}


int CarSensor::getSensor(byte sensorN) {
    return distanceSensors[sensorN];
}


int CarSensor::getGroupDistance(byte groupN) {
    if(groupN == 0) {
        if(distanceSensors[0] > distanceSensors[1] ) {
            distanceStation[0] = distanceSensors[0];
        } else {
            distanceStation[0] = distanceSensors[1];
        }
    } else if(groupN == 1) {
        if(distanceSensors[2] > distanceSensors[3] ) {
            distanceStation[1] = distanceSensors[2];
        } else {
            distanceStation[1] = distanceSensors[3];
        }
    }

    return distanceStation[groupN];
}


void CarSensor::printDebug() {

    if(flagSensorFound) {

        Serial.print("Pin: ");
        Serial.print(_pin);
        Serial.print(" - ");
        Serial.print(distanceStation[0]);
        Serial.print(",");
        Serial.print(distanceStation[1]);

        Serial.print("\t");

        for(int i=0;i<TOTAL_SENSORS;i++) {
            Serial.print(distanceSensors[i]);
            Serial.print(",");
        }

        Serial.print("\t");

        for(int i=0;i<TOTAL_SENSORS;i++) {
            if(proximitySensors[i] == 1) {
                Serial.print("PX");
            } else {
                Serial.print("--");
            }
            // Serial.print(proximitySensors[i]);
            Serial.print(".");
        }

        Serial.print("\t");

        for(int i=0;i<TOTAL_SENSORS;i++) {
            if(connectedSensors[i] == 1) {
                Serial.print("CO");
            } else {
                Serial.print("--");
            }
            Serial.print(".");
        }

        Serial.print("\t");

        for(int i=0;i<TOTAL_SENSORS;i++) {
            Serial.print(tempSensors[i]);
            Serial.print(",");
        }

        Serial.print("\t");


        for(int i=0;i<MAX_BITS-1;i++) {

                if(i==8 || i==16 || i==24 || i==32 || i==41 || i==43 || i==45 || i==47 || i==49) {
                     Serial.print(" ");
                }

                Serial.print(arrayBits[i]);

        }

        Serial.println(" ");


    } else {
        Serial.print("Pin: ");
        Serial.print(_pin);
        Serial.println(" - Module not found.");
    }
}
