#include <NextionX2.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial_2(11, 12); // RX, TX

NextionComPort nextion;
NextionComponent date(nextion, 0, 1);
NextionComponent temp(nextion, 0, 4);
NextionComponent pressure(nextion, 0, 8);

void setupDisplay() { nextion.begin(Serial_2); }

void updateDisplay() { nextion.update(); }