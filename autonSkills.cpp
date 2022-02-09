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
motor LeftLiftMotor = motor(PORT11, ratio18_1, true);

motor RightLiftMotor = motor(PORT20, ratio18_1, false);

controller Controller1 = controller(primary);
motor LeftRearMotor = motor(PORT10, ratio36_1, true);

motor RightRearMotor = motor(PORT1, ratio36_1, false);

motor LeftFrontMotor = motor(PORT9, ratio36_1, true);

motor RightFrontMotor = motor(PORT2, ratio36_1, false);


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

struct MotorController {
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

struct PairedMotorController {
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


PairedMotorController liftArms(LeftLiftMotor, RightLiftMotor);

PairedMotorController leftWheels(LeftRearMotor, LeftFrontMotor);
PairedMotorController rightWheels(RightRearMotor, RightFrontMotor);


struct ScheduledGoal {
    float t;
    float l;
    float r;
    float lift;

    bool armed = true;

    ScheduledGoal(float t, float l, float r, float lift=-1): t(t), l(l), r(r), lift(lift) {}

    void update() {
        if(this->armed && Brain.Timer.time(seconds) >= this->t) {
            this->armed = false;

            leftWheels.goal = this->l;
            rightWheels.goal = this->r;
            if(lift != -1) {
                liftArms.goal = this->lift;
            }
        }
    }
};
// ScheduledGoal moveForward(float t, float x=1) {return ScheduledGoal(t, x/18, x/18);};
// ScheduledGoal moveBackward(float t, float x=1) {return ScheduledGoal(t, -x/18, -x/18);};
// ScheduledGoal turnLeft(float t, float x=1) {return ScheduledGoal(t, -x*0.75, x*0.75);};
// ScheduledGoal turnRight(float t, float x=1) {return ScheduledGoal(t, x*0.75, -x*0.75);};


int main() {
    leftWheels.maxVolts = 6;
    rightWheels.maxVolts = 6;

    leftWheels.forceScale = 500;
    rightWheels.forceScale = 500;


    ScheduledGoal schedule[] = {
        // ScheduledGoal(2, 1, 1),
        // ScheduledGoal(4, 2, 2),
        // ScheduledGoal(6, 0, 0),
        // ScheduledGoal(8, 2, 2),
        // ScheduledGoal(10, 3, 1),
        // ScheduledGoal(12, 5, 3),
        // ScheduledGoal(14, 3, 1),
        // ScheduledGoal(16, 2, 2),
        // ScheduledGoal(18, 0, 0),

        // ScheduledGoal(2, 0.75, -0.75),

        // ScheduledGoal(2, 1, 1)

        // moveForward(2, 1),
        // moveBackward(4, 1),


        // Comp left

        // ScheduledGoal(0, 0.7, -0.7, 0.5),
        // ScheduledGoal(2, 0.7 + 1, -0.7 + 1),
        // ScheduledGoal(4, 0.7 + 1, -0.7 + 1, 0),
        // ScheduledGoal(6, 0.7, -0.7),
        // ScheduledGoal(8, 0, 0, 0),


        // Auton

        ScheduledGoal(0, 3, 3),
        ScheduledGoal(4, 3, 3, 0.25),
        ScheduledGoal(5, 0, 0),
        ScheduledGoal(9, 0.5, -0.5),
        ScheduledGoal(10, 0.5, -0.5, 0),
        ScheduledGoal(11, 0, 0),
        ScheduledGoal(12, 1.5, 1.5),
        ScheduledGoal(14, 1.125, 1.875),
        ScheduledGoal(15, 3.125, 3.875),
        ScheduledGoal(17, 1.125, 1.875),
    };


    while(true) {
        for(int i=0; i<10; i++) {
            schedule[i].update();
        }


        liftArms.update();
        leftWheels.update();
        rightWheels.update();

        wait(5, msec);
    }
}
