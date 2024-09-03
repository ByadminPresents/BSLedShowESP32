#ifndef BEATSABERMAINMENUEFFECT_H
#define BEATSABERMAINMENUEFFECT_H

#include "Effect.h"
#include <FastLED.h>

class BeatSaberMainMenuEffect : public Effect
{
private:
    static const int FirstMiddleIndex = 315, SecondMiddleIndex = 768, MiddleOffset = 8, StepsToFadeOut = 130;
    static const int RedColorR = 230, RedColorG = 32, RedColorB = 32;
    static const int BlueColorR = 32, BlueColorG = 100, BlueColorB = 168;
    int iteration = 0;
public:
    BeatSaberMainMenuEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : Effect(leds, ledsCount, params) 
    {

    }

    bool Iterate() override;
    
    bool TransitionIterate() override;

};


#endif // BEATSABERMAINMENUEFFECT_H