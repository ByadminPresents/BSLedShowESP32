#ifndef EFFECTUTILFUNCTIONS_H
#define EFFECTUTILFUNCTIONS_H

#include <stdlib.h>

inline int GetRandomValue(int minValue, int maxValue) {
    if (maxValue <= minValue) {
        return minValue;
    }
    return rand() % (maxValue - minValue + 1) + minValue;
}

inline double GetRandomValue(double minValue, double maxValue) {
    if (minValue >= maxValue) {
        return minValue;
    }
    
    double range = maxValue - minValue;
    return (rand() / (double)RAND_MAX) * range + minValue;
}

#endif // EFFECTUTILFUNCTIONS_H