#include "SolidColorEffect.h"

bool SolidColorEffect::Iterate()
{
    if (!stateIterated)
    {
        for (int i = 0; i < ledsCount; i++)
        {
            leds[i] = CRGB(r, g, b);
        }
        stateIterated = true;
    }
    return true;
}

bool SolidColorEffect::TransitionIterate()
{
    SolidColorEffect::Iterate();

    return true;
}