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

inline void PlaceColorInRange(CRGB* array, size_t arraySize, CRGB color, double fromIndex, double toIndex, double offset = 0)
{
    fromIndex += offset;
    toIndex += offset;

    int intFromIndex = (int)floor(fromIndex) % arraySize, intToIndex = (int)floor(toIndex) % arraySize;
    if (intFromIndex < 0)
    {
        intFromIndex = arraySize + intFromIndex;
    }

    if (intToIndex < 0)
    {
        intToIndex = arraySize + intToIndex;
    }

    for (int n = 0; n < 3; n++)
    {
        array[intFromIndex][n] = color[n] * pow(1 - abs((abs(fromIndex) - abs(floor(fromIndex)))), 1.3);
        array[(intToIndex + 1) % arraySize][n] = color[n] * pow(abs((abs(toIndex) - abs(floor(toIndex)))), 1.3);
    }

    for (int i = floor(fromIndex) + 1; i <= floor(toIndex); i++)
    {
        array[i % arraySize] = color;
    }
}

#endif // EFFECTUTILFUNCTIONS_H