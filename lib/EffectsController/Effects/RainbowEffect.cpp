#include "RainbowEffect.h"

bool RainbowEffect::Iterate()
{
    hueOffset = hue;
    hue += speed;
    if (oppositeDirection)
    {
        for (int i = ledsCount - 1; i >= 0; i--)
        {
            leds[i] = CHSV(hueOffset++, 255, 255);
        }
    }
    else
    {
        for (int i = 0; i < ledsCount; i++)
        {
            leds[i] = CHSV(hueOffset++, 255, 255);
        }
    }
    return true;
}

bool RainbowEffect::TransitionIterate()
{
    RainbowEffect::Iterate();

    return true;
}