/*
 * Cyber310.cpp
 *
 * Author: Gert Kremer (gert at huize-kremer dot nl)
 *
 * Implementation for the Cyber310 arduino controle routines. THis
 * is implemented as a library, allowing the Cyber310 arm to be
 * re-used in different sketches.
 */
 
/*
Atmega  Arduino  Function
     2        0  Move selected motors
     3        1  Base motor
     4        2  Shoulder motor
     5        3  Elbow motor
     6        4  Left wrist motor
    11        5  Right wrist motor
    12        6  Grip motor
    13        7  Set motor direction
*/
 
#include "Cyber310.h"


Cyber310::Cyber310()
{
  _init(A1,A2,A3,A4,A5,A6,A7,A0);
}

Cyber310::Cyber310(int base, int shoulder, int elbow, int lw, int rw, int grip, int dir, int move)
{
  _init(base,shoulder,elbow,lw,rw,grip,dir,move);
}

void Cyber310::move(byte motor, byte speed, byte dir, int steps)
{
  motor_speed[motor]=speed;
  motor_dir[motor]=dir;
  motor_steps[motor]=steps;
}

void Cyber310::tick()
{
  /* State drive scheme:
  0	dir + pin_dir(H)
  1	pin_dir(L)
  2	motor(2) + pin_move(H)
  3	pin_move(L)
  4	dir + pin_dir(H)
  5	pin_dir(L)
  6	motor(4) + pin_move(H)
  7	pin_move(L)
  8	dir + pin_dir(H)
  9	pin_dir(L)
 10	motor(1) + pin_move(H)
 11	pin_move(L)
 12	dir + pin_dir(H)
 13	pin_dir(L)
 14	motor(4) + pin_move(H)
 15	pin_move(L)
 
 The tick function is (ultimately) intended to be called from
 a timer interrupt (e.g. drive motors as background task)
 */
  switch(state) {
    case 0:
    case 4:
    case 8:
    case 12:
      _setDir();
      break;
    case 6:
    case 14:
      _driveMotor(FAST);
      break;
    case 2:
      _driveMotor(MEDIUM);
      break;
    case 10:
      _driveMotor(SLOW);
      break;
    case 1:
    case 5:
    case 9:
    case 13:
      digitalWrite(pin_dir,LOW);
      break;
    case 3:
    case 7:
    case 11:
    case 15:
      digitalWrite(pin_move,LOW);
      break;
  }    
  if ( ++state >= 16 ) state = 0;
}

void Cyber310::fullStop()
{
  byte i;
  
  for (i=0;i<NUM_MOTORS;i++) {
    motor_speed[i]=0;
    motor_steps[i]=0;
  }
  digitalWrite(pin_dir,LOW);
  digitalWrite(pin_move,LOW);
}

void Cyber310::_init(int base, int shoulder, int elbow, int lw, int rw, int grip, int dir, int move)
{
  _initPin(base,&(pins[BASE]));
  _initPin(shoulder, &(pins[SHOULDER]));
  _initPin(elbow, &(pins[ELBOW]));
  _initPin(lw, &(pins[LEFT_WRIST]));
  _initPin(rw,&(pins[RIGHT_WRIST]));
  _initPin(grip,&(pins[GRIP]));
 
  _initPin(dir,&pin_dir);
  _initPin(move,&pin_move); 
  state = 0;
 fullStop();
}

void Cyber310::_initPin(byte pin,  byte *store_loc)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  *store_loc = pin;
}

void Cyber310::_setDir()
{
  byte i;
  
  for (i=0;i<NUM_MOTORS;i++) {
    digitalWrite(pins[i], motor_dir[i]);
  }
  digitalWrite(pin_dir,HIGH); // write direction bits to arm, e.g enable strobe
}


void Cyber310::_driveMotor(byte min_speed)
{
  byte i;
  
  for (i=0;i<NUM_MOTORS;i++ ) {
    if ( motor_speed[i] >= min_speed ) {
      if ( motor_steps[i] ) {
        digitalWrite(pins[i],HIGH);
        motor_steps[i]--;
      }
    } else {
      digitalWrite(pins[i],LOW);
    }
  }
  digitalWrite(pin_move,HIGH);  // drive selected motors 1 step
}
