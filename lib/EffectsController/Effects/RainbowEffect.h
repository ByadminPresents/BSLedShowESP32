#ifndef RAINBOWEFFECT_H
#define RAINBOWEFFECT_H

#include "Effect.h"
#include <FastLED.h>

class RainbowEffect : public Effect
{
private:
    double hue = 0;
    int hueOffset;
    double speed;
    bool oppositeDirection = false;
public:
    RainbowEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params) 
    {
        speed = params[1] / 100.0;
        oppositeDirection = (bool)params[0];
    };

    bool Iterate() override;
    
    bool TransitionIterate() override;
};

#endif // RAINBOWEFFECT_H