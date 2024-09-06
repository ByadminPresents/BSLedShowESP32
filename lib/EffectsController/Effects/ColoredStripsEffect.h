#ifndef COLOREDSTRIPSEFFECT_H
#define COLOREDSTRIPSEFFECT_H

#include "Effect.h"
#include <FastLED.h>
#include <vector>

class ColoredStripsEffect : public Effect
{
private:
    bool oppositeDirection = false;
    double speed, colorStripLength = 0, offset = 0;
    int countOfBatches, countOfColors, range = 1, spacing = 2;
    std::vector<CRGB> colors;
public:
    ColoredStripsEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params) 
    {
        oppositeDirection = (bool)params[0];
        speed = params[1] / 100.0;
        countOfBatches = params[2];
        range = abs((int)params[3]);
        spacing = abs((int)params[4]);
        countOfColors = params[5];
        colors.reserve(countOfColors);
        for (int i = 6; i < countOfColors * 3 + 6; i += 3)
        {
            colors.emplace_back(params[i], params[i + 1], params[i + 2]);
        }
        colorStripLength = (ledsCount - (countOfBatches * range) - (countOfBatches * (countOfColors - 1) * spacing)) / (double)(countOfBatches * countOfColors);
    }

    bool Iterate() override;
    
    bool TransitionIterate() override;

};


#endif // COLOREDSTRIPSEFFECT_H