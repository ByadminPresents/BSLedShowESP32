#ifndef EFFECT_H
#define EFFECT_H

#include "cstdint"
#include <FastLED.h>

class Effect
{
protected:
    CRGB *leds;
    uint16_t ledsCount;
    uint32_t *params;
public:
    Effect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : leds(leds), ledsCount(ledsCount), params(params) {}

    virtual bool Iterate();

    virtual bool TransitionIterate();

    virtual ~Effect() {
        free(params);
    }
};

#endif // EFFECT_H