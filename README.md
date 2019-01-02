## CarSensor

  Arduino IDE Library to read cheap ultrasonic car parking sensors.

  NOTE: This repository still under construction. Soon it will have complete information about the module hardware and data formats.

```c
#include "CarSensor.h"

#define PIN_MODULE 4

CarSensor carModule(PIN_MODULE);

void setup() {
    Serial.begin(115200);
    Serial.println("Module Begin");
    carModule.begin(); // initialize and calibrate
}

void loop() {
    carModule.read(); // get data from module
    int sensor1 = carModule.getSensor(0);
}

```

## Pulse Train

The pulse train consists of 50 bits. The first 4 bytes are the sensors readings. They come in a strange order:

Pulse train example:
00000000 00000000 00000000 00000000 000010010 11 00 00 00

First byte 	- sensor A
Second byte - sensor C
Third byte 	- sensor B
Fourth byte - sensor D

## Compatibility

	* Arduinos (UNO & Pro-micro tested)
	* ESP32 (Lolin32 and ESP32 Dev. Module tested). Remember, these modules are 3.3V. Be sure to make a voltage divider with 2 resistors.




