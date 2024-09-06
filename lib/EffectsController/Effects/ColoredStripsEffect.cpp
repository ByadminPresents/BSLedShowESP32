#include "ColoredStripsEffect.h"
#include "EffectUtilFunctions.h"

bool ColoredStripsEffect::Iterate()
{
    int currentCountOfRanges = 0, currentCountOfSpacings = 0;
    for (int i = 0; i < ledsCount; i++)
    {
        for (int n = 0; n < 3; n++)
        {
            leds[i][n] = 0;
        }
    }

    for (int i = 0; i < countOfBatches; i++)
    {
        for (int n = 0; n < countOfColors; n++)
        {
            double totalColorsOffset = (i * countOfColors + n) * colorStripLength + currentCountOfRanges * range + currentCountOfSpacings * spacing;
            PlaceColorInRange(leds, ledsCount, colors[n], totalColorsOffset, totalColorsOffset + colorStripLength, offset);
            if (n + 1 != countOfColors)
            {
                currentCountOfSpacings++;
            }
        }
        currentCountOfRanges++;
    }
    
    if (oppositeDirection)
    {
        offset -= speed;
    }
    else
    {
        offset += speed;
    }

    if (offset < 0)
    {
        offset += ledsCount;
    }

    if (offset > ledsCount)
    {
        offset -= ledsCount;
    }

    return true;
    
}

bool ColoredStripsEffect::TransitionIterate()
{
    return Iterate();
}
