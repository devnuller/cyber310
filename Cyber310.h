#include <Arduino.h>
#include <avr/io.h>

// Motor names 
#define BASE	 	0
#define SHOULDER 	1
#define ELBOW	 	2
#define LEFT_WRIST	3
#define RIGHT_WRIST	4
#define GRIP	 	5

#define NUM_MOTORS	6

// Direction names
#define FORWARD	0
#define REVERSE 1

// Speed names
#define STOP	0
#define SLOW	1
#define MEDIUM	2
#define FAST	4

class Cyber310
{
private:
  byte motor_speed[NUM_MOTORS];
  byte motor_dir[NUM_MOTORS];
  int motor_steps[NUM_MOTORS];
  byte pins[NUM_MOTORS];
  byte state;  // internal state number, cycles from 0-15 and is driven by tick()
  byte pin_dir;
  byte pin_move;
  void _init(int base, int shoulder, int elbow, int lw, int rw, int grip, int dir, int move);
  void _initPin(byte pin,  byte *store_loc);
  void _setDir();
  void _driveMotor(byte min_speed);
public:
  Cyber310();
  Cyber310(int base, int shoulder, int elbow, int lw, int rw, int grip, int dir, int move);
  void move(byte motor, byte speed, byte direction, int steps);
  void tick();
  void fullStop();

};
