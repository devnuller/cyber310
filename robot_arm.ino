//#include <PS2X_lib.h>

#include "Cyber310.h"


Cyber310 arm(A1,A2,A3,A4,A5,A6,A7,A0);
//PS2X stick;

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  byte i;
  
  
  arm.move(BASE,FAST,FORWARD,16);
  
  Serial.write("Going forward\n");
  // drive motors through one cycle of 16 states
  for (i=0;i<128;i++) {
    arm.tick();
    delayMicroseconds(5000);
  }
  
  delay(5000);

  arm.move(BASE,FAST,REVERSE,16);
  Serial.write("Going back\n");
  // drive motors through one cycle of 16 states
  for (i=0;i<128;i++) {
    arm.tick();
    delayMicroseconds(5000);
  }
  
  delay(5000);
  
}
