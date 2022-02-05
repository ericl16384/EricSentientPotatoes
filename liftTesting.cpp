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
motor LeftMotor = motor(PORT11, ratio18_1, false);

motor RightMotor = motor(PORT20, ratio18_1, true);

controller Controller1 = controller(primary);

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

    PairedMotorController lift(LeftMotor, RightMotor);

    lift.goal = 1;

    while(true) {
        if(Controller1.ButtonR1) {
            lift.goal += 0.01;
        }
        if(Controller1.ButtonR2) {
            lift.goal -= 0.01;
        }
        
        if(Controller1.ButtonB) {
            lift.goal = 0;
        }
        if(Controller1.ButtonA) {
            lift.goal = 0.5;
        }
        if(Controller1.ButtonY) {
            lift.goal = 1;
        }
        if(Controller1.ButtonX) {
            lift.goal = 1.5;
        }

        lift.update();

        wait(5, msec);
    }
}
