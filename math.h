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
