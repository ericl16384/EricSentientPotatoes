#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor leftMotorA = motor(PORT9, ratio18_1, false);
motor leftMotorB = motor(PORT10, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT2, ratio18_1, true);
motor rightMotorB = motor(PORT1, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
gps DrivetrainGPS = gps(PORT20, 0.00, 0.00, mm, 180);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainGPS, 319.19, 320, 40, mm, 1);

gps GPS3 = gps(PORT3, 0.00, 0.00, mm, 180);

void calibrateDrivetrain() {
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("GPS");
  DrivetrainGPS.calibrate();
  while (DrivetrainGPS.isCalibrating()) {
    wait(25, msec);
  }

  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}
#pragma endregion VEXcode Generated Robot Configuration

#include "vex.h"
#include <cmath>
  
// Allows for easier use of the VEX Library
using namespace vex;

float myVariable;


//these are for the raduis of the robot and two angles. 
//These are just so if the size of the bot changes, you just have to change r (radius). 
// 
float r = 8;

//# define drive(x) Drivetrain.driveFor(forward, x, inches)
//# define turn(x)

//all the movement
int main() {
    Drivetrain.driveFor(forward, 23.5 - r, inches);
    // grab
    Drivetrain.driveFor(forward, -23.5 + r, inches);

    Drivetrain.turnFor(left, 90, degrees);
    Drivetrain.driveFor(forward, 58.9, inches);

    Drivetrain.turnFor(right, 33.58, degrees);
    Drivetrain.driveFor(forward, 42.49, inches);
    // drop

    Drivetrain.turnFor(left, 123.58, degrees);
    Drivetrain.driveFor(forward, 7 - r, inches);
    // grab to storage
    Drivetrain.driveFor(forward, 6.49 + r, inches);

    Drivetrain.turnFor(left, 90, degrees);
    Drivetrain.driveFor(forward, 35.4, inches);

    Drivetrain.turnFor(left, 90, degrees);
    Drivetrain.driveFor(forward, 17 - r, inches);
    // grab to storage
    Drivetrain.driveFor(forward, 35.3, inches);
    // grab to storage
    Drivetrain.driveFor(forward, 36, inches);
    // grab to storage
    Drivetrain.driveFor(forward, 6.49 + r, inches);

    Drivetrain.turnFor(right, 90, degrees);
    Drivetrain.driveFor(forward, 58.9, inches);

    Drivetrain.turnFor(right, 90, degrees);
    // will need to pull the ramp down
    Drivetrain.driveFor(forward, 35.4, inches);
}


//Austin, I want't sure how the lift is meant to be coded. I just left what is supposed to happen in comments.
// Me and Eric can probobly add in that stuff relatively quickly. The gap is just a small part of the movemenst.
// I need to see the feild and test before I can code that. If you put in the variables, it should work.
