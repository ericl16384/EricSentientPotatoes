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
motor LeftLiftMotor = motor(PORT2, ratio18_1, false);

motor RightLiftMotor = motor(PORT9, ratio18_1, true);

controller Controller1 = controller(primary);
motor LeftRearMotor = motor(PORT3, ratio36_1, true);

motor RightRearMotor = motor(PORT8, ratio36_1, false);

motor LeftFrontMotor = motor(PORT1, ratio36_1, true);

motor RightFrontMotor = motor(PORT10, ratio36_1, false);


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

bool within(float x, float min, float max) {
    if(x < min) {
        return false;
    } else if(x > max) {
        return false;
    } else {
        return true;
    }
}

bool threshold(float x, float goal, float tolerance) {
    return within(x, goal - tolerance, goal + tolerance);
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

    bool inThreshold(float tolerance) {
        if(!threshold(this->m.position(turns), this->goal, tolerance)) {
            return false;
        } else {
            return true;
        }
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

    bool inThreshold(float tolerance) {
        if(!threshold(this->m1.position(turns), this->goal, tolerance)) {
            return false;
        } else if(!threshold(this->m2.position(turns), this->goal, tolerance)) {
            return false;
        } else {
            return true;
        }
    }
};


PairedMotorController liftArms(LeftLiftMotor, RightLiftMotor);
PairedMotorController leftWheels(LeftRearMotor, LeftFrontMotor);
PairedMotorController rightWheels(RightRearMotor, RightFrontMotor);


struct AutonomousAction {
    float liftGoal=0, leftGoal=0, rightGoal=0;
    bool liftRelative=true, leftRelative=true, rightRelative=true;

    float tolerance = 0.05;
    float waiting = 0.5;

    // AutonomousAction(
    //     float liftGoal, float leftGoal, float rightGoal,
    //     bool liftRelative=true, bool leftRelative=true,  bool rightRelative=true,
    //     float minTime=0
    // )
    // AutonomousAction() {}
    // AutonomousAction(AutonomousAction previous) {
    //     this->liftGoal = previous.liftGoal;
    //     this->leftGoal = previous.leftGoal;
    //     this->rightGoal = previous.rightGoal;

    //     this->liftRelative = previous.liftRelative;
    //     this->leftRelative = previous.leftRelative;
    //     this->rightRelative = previous.rightRelative;

    //     this->minTime = previous.minTime;
    // } 

    void apply() {
        if(this->liftRelative) {
            liftArms.goal += this->liftGoal;
        } else {
            liftArms.goal = this->liftGoal;
        }

        if(this->leftRelative) {
            leftWheels.goal += this->leftGoal;
        } else {
            leftWheels.goal = this->leftGoal;
        }

        if(this->rightRelative) {
            rightWheels.goal += this->rightGoal;
        } else {
            rightWheels.goal = this->rightGoal;
        }
    }

    bool done() {
        if(!liftArms.inThreshold(this->tolerance)) {
            return false;
        } else if(!leftWheels.inThreshold(this->tolerance)) {
            return false;
        } else if(!rightWheels.inThreshold(this->tolerance)) {
            return false;
        } else {
            return true;
        }
    }
};

struct Autonomous {
    AutonomousAction actions[1024];
    int actionsLength = 0;

    int currentAction = -1;
    float actionStart = 0;
    // float actionEnd = -1;

    void addAction(AutonomousAction action) {
        this->actions[this->actionsLength] = action;
        this->actionsLength++;
    }

    void startNextAction() {
        if(!this->finished()) {
            this->currentAction++;
            this->actionStart = Brain.Timer.time(seconds);   
            // this->actionEnd = this->actionStart + this->actions[this->currentAction].minTime;
            // this->actionEnd = -1;
            this->actions[this->currentAction].apply();
        }
    }

    void update() {
        // if(this->actions[this->currentAction].done() && this->actionEnd == -1) {
        //     this->actionEnd = Brain.Timer.time(seconds) + this->actions[this->currentAction].waiting;
        // } else if(Brain.Timer.time(seconds) >= this->actionEnd && this->actionEnd != -1) {
        //     this->startNextAction();
        // }

        if(this->currentAction == -1 || this->actions[this->currentAction].done()) {
            this->startNextAction();
        }

        Brain.Screen.clearScreen();
        Brain.Screen.print(this->currentAction);
    }

    bool finished() {
        return this->currentAction >= this->actionsLength;
    }


    void addMove(float dist) {
        AutonomousAction action;
        action.leftGoal = dist;
        action.rightGoal = dist;
        this->addAction(action);
    }

    // replace amt with theta, when I figure out the relationship
    // between the amt and theta
    void addTurn(float amt) {
        AutonomousAction action;
        action.leftGoal = -amt;
        action.rightGoal = amt;
        this->addAction(action);
    }

    void addLift(float goal) {
        AutonomousAction action;
        action.liftGoal = goal;
        action.liftRelative = false;
        this->addAction(action);
    }
};


void enableAuton() {
    RemoteControlCodeEnabled = false;
    leftWheels.maxVolts = 6;
    rightWheels.maxVolts = 6;
}
void enableDrive() {
    RemoteControlCodeEnabled = true;
    leftWheels.maxVolts = 12;
    rightWheels.maxVolts = 12;
}

int main() {
    competition comp;
    comp.autonomous(enableAuton);
    comp.drivercontrol(enableDrive);


    Autonomous auton;

    // first goal
    auton.addMove(3);
    auton.addLift(0.25);
    auton.addMove(-3);
    auton.addTurn(-0.5);
    auton.addLift(0);
    auton.addTurn(0.5);

    // second goal
    // auton.addMove(1.5);
    // auton.addTurn(0.125);
    // auton.addMove(2);
    // auton.addLift(0.25);
    // auton.addMove(-2);
    // auton.addTurn(-0.125);
    // auton.addMove(-1.5);
    // auton.addLift(0);

    // Brain.Screen.print(sizeof(auton.actions));
    // Brain.Screen.newLine();
    // Brain.Screen.print(sizeof(auton.actions[0]));
    // Brain.Screen.newLine();
    // Brain.Screen.print(auton.actionsLength);
    // Brain.Screen.newLine();

    // replace with Competition callbacks
    // bool driveMode = false;
    // RemoteControlCodeEnabled = true;

    // leftWheels.maxVolts = 6;
    // rightWheels.maxVolts = 6;
    leftWheels.forceScale = 500;
    rightWheels.forceScale = 500;

    while(true) {
        // if(Controller1.ButtonL1.pressing()) {driveMode = false;}
        // if(Controller1.ButtonL2.pressing()) {driveMode = true;}


        // DEBUG!!!
        if(Controller1.ButtonL1.pressing() && Controller1.ButtonL2.pressing()) {
            enableAuton();
        }


        // if(driveMode) {
        if(RemoteControlCodeEnabled) {
            if(Controller1.ButtonR1.pressing()) {liftArms.goal += 0.01;}
            if(Controller1.ButtonR2.pressing()) {liftArms.goal -= 0.01;}
            if(Controller1.ButtonX.pressing()) {liftArms.goal = 1.5;}
            if(Controller1.ButtonY.pressing()) {liftArms.goal = 1;}
            if(Controller1.ButtonA.pressing()) {liftArms.goal = 0.5;}
            if(Controller1.ButtonB.pressing()) {liftArms.goal = 0;}
            liftArms.goal = constrain(liftArms.goal, 0, 1.5);

            float l = Controller1.Axis3.position() + Controller1.Axis4.position();
            float r = Controller1.Axis3.position() - Controller1.Axis4.position();

            l /= 8;
            r /= 8;

            l = constrain(l, -12, 12);
            r = constrain(r, -12, 12);

            l *= 1 - (liftArms.goal / 1.5 * 0.65);
            r *= 1 - (liftArms.goal / 1.5 * 0.65);

            LeftFrontMotor.spin(forward, l, volt);
            LeftRearMotor.spin(forward, l, volt);
            RightFrontMotor.spin(forward, r, volt);
            RightRearMotor.spin(forward, r, volt);
        } else {
            auton.update();
        }


        liftArms.update();
        // if(!driveMode) {
        if(!RemoteControlCodeEnabled) {
            leftWheels.update();
            rightWheels.update();

            // Debug?
            if(auton.finished()) {
                enableDrive();
            }
        }

        wait(5, msec);
    }
}
