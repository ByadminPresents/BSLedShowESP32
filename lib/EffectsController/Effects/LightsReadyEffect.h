#ifndef LIGHTSREADYEFFECT_H
#define LIGHTSREADYEFFECT_H

#include "Effect.h"
#include <FastLED.h>

static int InterpolateIndexes(int fromIndex, int toIndex, int f, int fMax);

class LightsReadyEffect : public Effect
{
private:
    
    static const int centerIndex = 315, leftCorner = 661, rightCorner = 0, thresholdIteration = 600, stopThreshold = 930, brightnessOffset = 300;
    bool isBufferInitialized = false;
    int iteration = 0;
public:
    LightsReadyEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params) 
    {

    }

    bool Iterate() override;
    
    bool TransitionIterate() override;

};

#endif // LIGHTSREADYEFFECT_H