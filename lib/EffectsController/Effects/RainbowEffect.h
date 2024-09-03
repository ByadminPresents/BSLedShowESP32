#ifndef RAINBOWEFFECT_H
#define RAINBOWEFFECT_H

#include "Effect.h"
#include <FastLED.h>

class RainbowEffect : public Effect
{
private:
    double hue;
    int hueOffset;
    double speed;
    bool oppositeDirection;
public:
    RainbowEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params) 
    {
        hue = 0;
        oppositeDirection = false;

        speed = params[1] / 100.0;
        if (params[0] == 1)
        {
            oppositeDirection = true;
        }
    };

    bool Iterate() override;
    
    bool TransitionIterate() override;
};

#endif // RAINBOWEFFECT_H