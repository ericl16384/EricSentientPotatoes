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
motor LeftLiftMotor = motor(PORT11, ratio18_1, false);

motor RightLiftMotor = motor(PORT20, ratio18_1, true);

controller Controller1 = controller(primary);
motor LeftRearMotor = motor(PORT1, ratio36_1, false);

motor RightRearMotor = motor(PORT10, ratio36_1, false);

motor LeftFrontMotor = motor(PORT2, ratio36_1, false);

motor RightFrontMotor = motor(PORT9, ratio36_1, false);


// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;




#pragma endregion VEXcode Generated Robot Configuration
// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;


float abs(float x) {
    if(x < 0) {
        return -x;
    } else {
        return x;
    }
}

float constrain(float x, float min, float max) {
    if(x < min) {
        return min;
    } else if(x > max) {
        return max;
    } else {
        return x;
    }
}


float moveMotorTo(motor m, float goal, float forceScale, float maxVolts=12) {
    float delta = goal - m.position(turns);
    float volts = constrain(delta * forceScale, -maxVolts, maxVolts);

    m.spin(forward, volts, volt);
    return volts;
}

class MotorController {
    public:
        motor m;

        float goal;
        float forceScale = 20;
        float maxVolts = 12;

        MotorController(motor m): m(m) {
            this->goal = this->m.position(turns);
        }

        void update() {
            moveMotorTo(this->m, this->goal, this->forceScale, this->maxVolts);
        }
};

class PairedMotorController {
    public:
        motor m1;
        motor m2;

        float goal;
        float forceScale = 20;
        float maxVolts = 12;

        PairedMotorController(motor m1, motor m2): m1(m1), m2(m2) {
            this->goal = (m1.position(turns) + m2.position(turns)) / 2;
        }

        void update() {
            moveMotorTo(this->m1, this->goal, this->forceScale, this->maxVolts);
            moveMotorTo(this->m2, this->goal, this->forceScale, this->maxVolts);
        }
};


int main() {
    // wait for rotation sensor to fully initialize
    wait(30, msec);

    PairedMotorController lift(LeftLiftMotor, RightLiftMotor);

    PairedMotorController leftWheels(LeftRearMotor, LeftFrontMotor);
    PairedMotorController rightWheels(RightRearMotor, RightFrontMotor);

    leftWheels.maxVolts = 4;
    rightWheels.maxVolts = 4;

    // lift.goal = 1;

    while(true) {
        if(Controller1.ButtonR1.pressing()) {
            lift.goal += 0.01;
        }
        if(Controller1.ButtonR2.pressing()) {
            lift.goal -= 0.01;
        }
        if(Controller1.ButtonB.pressing()) {
            lift.goal = 0;
        }
        if(Controller1.ButtonA.pressing()) {
            lift.goal = 0.5;
        }
        if(Controller1.ButtonY.pressing()) {
            lift.goal = 1;
        }
        if(Controller1.ButtonX.pressing()) {
            lift.goal = 1.5;
        }

        // Note: this is not good, responsive user drive code
        // this is just a way to test auton movement, via the controller
        // if the wheels get behind where they should be, they will speed up
        // user control should link the axis to the voltage applied
        leftWheels.goal += Controller1.Axis3.position() / 100;
        rightWheels.goal += Controller1.Axis2.position() / 100;

        lift.update();

        wait(5, msec);
    }
}
