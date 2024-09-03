#ifndef SOLIDCOLOREFFECT_H
#define SOLIDCOLOREFFECT_H

#include "Effect.h"
#include <FastLED.h>

class SolidColorEffect : public Effect
{
private:
    byte r = 0, g = 0, b = 0;
    bool stateIterated = false;
public:
    SolidColorEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params)
    {
        r = params[0];
        g = params[1];
        b = params[2];
    };

    bool Iterate() override;

    bool TransitionIterate() override;
};

#endif // SOLIDCOLOREFFECT_H