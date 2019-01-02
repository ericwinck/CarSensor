#include "CarSensor.h"

#define PIN_MODULE 4

CarSensor carModule(PIN_MODULE);

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Sensors Begin");
    carModule.begin();
}


void loop() {
    
    // makes the sensor reading. Must be in the loop.
    carModule.read();

    // assign the sensor data to integers
    sensor1 = carModule.getSensor(0);
    sensor2 = carModule.getSensor(1);
    sensor3 = carModule.getSensor(2);
    sensor4 = carModule.getSensor(3);

    // enables the debug which prints the pulse train bits to the serial monitor
    // carModule.printDebug();

    // prints the sensor readings
    Serial.print("Sensor1: ");
    Serial.println(sensor1);

    Serial.print("Sensor2: ");
    Serial.println(sensor2);

    Serial.print("Sensor3: ");
    Serial.println(sensor3);

    Serial.print("Sensor4: ");
    Serial.println(sensor4);

    // Optional delay just to slow the printing on the monitor. 
    // The data coming from the module is faster and the library is non-blocking.
    delay(1000);

}
