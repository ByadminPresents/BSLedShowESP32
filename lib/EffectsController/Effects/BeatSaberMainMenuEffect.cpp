#include "BeatSaberMainMenuEffect.h"
#include "EffectUtilFunctions.h"

bool BeatSaberMainMenuEffect::Iterate()
{
    bool isRedColor = false;
    for (int i = 0; i < ledsCount; i++)
    {
        float brightness = 1;
        if (i == FirstMiddleIndex)
        {
            isRedColor = true;
        }
        if (i == SecondMiddleIndex)
        {
            isRedColor = false;
        }

        if (i >= FirstMiddleIndex - MiddleOffset - StepsToFadeOut && i <= FirstMiddleIndex - MiddleOffset)
        {
            brightness = abs(FirstMiddleIndex - MiddleOffset - i) / (float)StepsToFadeOut;
        }

        if (i >= FirstMiddleIndex - MiddleOffset && i <= FirstMiddleIndex + MiddleOffset)
        {
            brightness = 0;
        }

        if (i >= FirstMiddleIndex + MiddleOffset && i <= FirstMiddleIndex + MiddleOffset + StepsToFadeOut)
        {
            brightness = abs(FirstMiddleIndex + MiddleOffset - i) / (float)StepsToFadeOut;
        }

        if (i >= SecondMiddleIndex - MiddleOffset - StepsToFadeOut && i <= SecondMiddleIndex - MiddleOffset)
        {
            brightness = abs(SecondMiddleIndex - MiddleOffset - i) / (float)StepsToFadeOut;
        }

        if (i >= SecondMiddleIndex - MiddleOffset && i <= SecondMiddleIndex + MiddleOffset)
        {
            brightness = 0;
        }

        if (i >= SecondMiddleIndex + MiddleOffset && i <= SecondMiddleIndex + MiddleOffset + StepsToFadeOut)
        {
            brightness = abs(SecondMiddleIndex + MiddleOffset - i) / (float)StepsToFadeOut;
        }

        if (isRedColor)
        {
            leds[i].r = RedColorR * brightness;
            leds[i].g = RedColorG * brightness;
            leds[i].b = RedColorB * brightness;
        }
        else
        {
            leds[i].r = BlueColorR * brightness;
            leds[i].g = BlueColorG * brightness;
            leds[i].b = BlueColorB * brightness;
        }
        
    }
    return true;
}

bool BeatSaberMainMenuEffect::TransitionIterate()
{
    return Iterate();
}