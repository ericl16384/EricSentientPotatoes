#include "vex.h"
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
