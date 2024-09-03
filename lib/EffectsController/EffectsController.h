#ifndef EFFECTSCONTROLLER
#define EFFECTSCONTROLLER

#include "Effects/Effects.h"
#include <FastLed.h>

class EController
{
private:
    CRGB *leds;
    uint16_t ledsCount;

    Effect *firstEffect = NULL;
    CRGB *firstLEDBuffer;

    Effect *secondEffect = NULL;
    CRGB *secondLEDBuffer;

    byte queuedEffectId;
    uint32_t *queuedEffectParams = NULL;

    float effectTransitionWeight;
    float effectTransitionSpeed;
    bool transitionDirectionToFirstEffect = true, isTransitionRunning = false;

    void IterateOverEffect(CRGB *LEDbuffer, byte effectId, uint32_t *effectParams);

    bool ChangeWeight(bool downDirection);

    void SendWeightedColors(CRGB *from, CRGB *to);
    void SendColors(CRGB *leds);

public:
    EController(CRGB *leds, int ledsCount) : leds(leds), ledsCount(ledsCount)
    {
        firstLEDBuffer = (CRGB*)malloc(ledsCount * sizeof(*firstLEDBuffer));
        secondLEDBuffer = (CRGB*)malloc(ledsCount * sizeof(*secondLEDBuffer));

        currentLEDBuffer = firstLEDBuffer;

        for (int i = 0; i < ledsCount; i++)
        {
            firstLEDBuffer[i] = CRGB(0, 0, 0);
            secondLEDBuffer[i] = CRGB(0, 0, 0);
        }
        effectTransitionWeight = 0;
        effectTransitionSpeed = 0.01;
    }

    CRGB *currentLEDBuffer;

    ~EController()
    {
        if (queuedEffectParams != NULL)
        {
            free(queuedEffectParams);
        }
        if (firstLEDBuffer != NULL)
        {
            free(firstLEDBuffer);
        }
        if (secondLEDBuffer != NULL)
        {
            free(secondLEDBuffer);
        }
    }
    
    void IterateOverEffects();
    
    void SwitchEffect(byte effectId, bool implaceEffect, uint32_t *effectParams);
};

//extern EController EffectsController;

#endif // EFFECTSCONTROLLER