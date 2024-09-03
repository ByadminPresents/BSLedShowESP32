#include "LightsReadyEffect.h"
#include "EffectUtilFunctions.h"

bool LightsReadyEffect::Iterate()
{
    // if (!isBufferInitialized)
    // {
    //     for (int i = 0; i < ledsCount; i++)
    //     {
    //         for (int j = 0; j < 3; j++)
    //         {
    //             leds[i][j] = 0;
    //         }
    //     }
    //     isBufferInitialized = true;
    // }
    if (iteration > stopThreshold + 100)
    {
        return false;
    }
    iteration++;
    if (iteration < thresholdIteration)
    {
        double brightness = 0;
        int from = InterpolateIndexes(centerIndex, rightCorner - 60, iteration, thresholdIteration);
        int to = InterpolateIndexes(centerIndex, rightCorner - 60, iteration - 1, thresholdIteration) + 40;
        
        for (int i = from; i < to; i++)
        {
            if (i > from + 5)
            {
                brightness += (brightness + 8) / 8;
            }
            if (brightness > 255)
            {
                brightness = 255;
            }
            if (i < 0 || i >= centerIndex)
            {
                continue;
            }
            for (int j = 0; j < 3; j++)
            {
                leds[i][j] = max((int)leds[i][j], (int)brightness);
            }
        }
        brightness = 255; 
        from = InterpolateIndexes(centerIndex, leftCorner + 60, iteration - 1, thresholdIteration) - 40;
        to = InterpolateIndexes(centerIndex, leftCorner + 60, iteration, thresholdIteration);
        for (int i = from; i < to; i++)
        {
            if (i > from + 5)
            {
                brightness -= (brightness + 8) / 8;
            }
            if (brightness < 0)
            {
                brightness = 0;
            }
            if (i < centerIndex || i > leftCorner)
            {
                continue;
            }
            for (int j = 0; j < 3; j++)
            {
                leds[i][j] = max((int)leds[i][j], (int)brightness);
            }
        }
    }

    if (iteration >= thresholdIteration - brightnessOffset && iteration < stopThreshold && iteration % 7 == 0)
    {
        double brightness = 1;
        if (iteration <= thresholdIteration)
        {
            brightness = pow(iteration - (thresholdIteration - brightnessOffset), 2.6) / pow(brightnessOffset, 2.6);
        }
        if (iteration >= stopThreshold - brightnessOffset)
        {
            brightness = pow(brightnessOffset - (iteration - (stopThreshold - brightnessOffset)), 2.6) / pow(brightnessOffset, 2.6);
        }
        for (int i = 0; i < 4; i++)
        {
            int index = leftCorner + 6 * GetRandomValue(0, (ledsCount - 6 - leftCorner) / 6);
            for (int n = index; n < index + 6; n++)
            {
                if (n >= ledsCount)
                {
                    break;
                }
                leds[n].r = 255 * brightness;
                leds[n].g = 0;
                leds[n].b = 255 * brightness;
            }
        }
    }

    for (int i = 0; i < ledsCount; i++)
    {
        double divider = 14.0;
        if (iteration > thresholdIteration - brightnessOffset && i >= leftCorner)
        {
            divider = 4.0;
        }

        for (int j = 0; j < 3; j++)
        {
            if ((int)leds[i][j] - (leds[i][j] + 14) / divider > 0)
            {
                leds[i][j] -= (leds[i][j] + 14) / divider;
            }
            else
            {
                leds[i][j] = 0;
            }
        }
    }

    if (iteration == stopThreshold + 100)
    {
        for (int i = leftCorner; i < ledsCount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                leds[i][j] = 0;
            }
        }
    }

    return true;
}

bool LightsReadyEffect::TransitionIterate()
{
    // if (!isBufferInitialized)
    // {
    //     for (int i = 0; i < ledsCount; i++)
    //     {
    //         for (int j = 0; j < 3; j++)
    //         {
    //             leds[i][j] = 0;
    //         }
    //     }
    //     isBufferInitialized = true;
    // }
    return true;
}

static int InterpolateIndexes(int fromIndex, int toIndex, int f, int fMax)
{
    int x = 0;
    if (fromIndex > toIndex)
    {
        x = fromIndex - toIndex;
        return fromIndex - floor(f * (double)x / fMax);
    }
    x = toIndex - fromIndex;
    return floor(fromIndex + f * (double)x / fMax);
}