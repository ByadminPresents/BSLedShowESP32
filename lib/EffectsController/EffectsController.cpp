#include "EffectsController.h"

void EController::IterateOverEffects() {
    bool forceSendColors = false;
    if (queuedEffectParams != NULL)
    {
        if (firstEffect == NULL)
        {
            currentLEDBuffer = firstLEDBuffer;
            firstEffect = GetEffectById(queuedEffectId, firstLEDBuffer, ledsCount, queuedEffectParams);
            queuedEffectParams = NULL;
            transitionDirectionToFirstEffect = true;
            effectTransitionStartTimestamp = millis();
            isTransitionRunning = true;
        }
        if (secondEffect == NULL)
        {
            currentLEDBuffer = secondLEDBuffer;
            secondEffect = GetEffectById(queuedEffectId, secondLEDBuffer, ledsCount, queuedEffectParams);
            queuedEffectParams = NULL;
            transitionDirectionToFirstEffect = false;
            effectTransitionStartTimestamp = millis();
            isTransitionRunning = true;
        }
    }

    if (firstEffect != NULL)
    {
        if (isTransitionRunning)
        {
            firstEffect->TransitionIterate();
        }
        else
        {
            firstEffect->Iterate();
        }
    }
    if (secondEffect != NULL)
    {
        if (isTransitionRunning)
        {
            secondEffect->TransitionIterate();
        }
        else
        {
            secondEffect->Iterate();
        }
    }

    if (isTransitionRunning)
    {
        if (SendWeightedColors(secondLEDBuffer, firstLEDBuffer)) {
            if (transitionDirectionToFirstEffect)
            {
                if (secondEffect != NULL)
                {
                    delete secondEffect;
                    secondEffect = NULL;
                }
            }
            else
            {
                if (firstEffect != NULL)
                {
                    delete firstEffect;
                    firstEffect = NULL;
                }
            }
            isTransitionRunning = false;
        }
    }
    else
    {
        if (transitionDirectionToFirstEffect)
        {
            SendColors(firstLEDBuffer);
        }
        else
        {
            SendColors(secondLEDBuffer);
        }
    }

    //Serial.println(effectTransitionStartTimestamp);
}

bool EController::SendWeightedColors(CRGB *from, CRGB *to)
{
    bool transitionIsEndFlag = false;
    double effectTransitionWeight = (millis() - effectTransitionStartTimestamp) / (double)effectTransitionDelayMillis;
    if (effectTransitionWeight >= 1)
    {
        effectTransitionWeight = 1;
        transitionIsEndFlag = true;
    }

    if (transitionDirectionToFirstEffect)
    {
        effectTransitionWeight = 1 - effectTransitionWeight;
    }

    for (int i = 0; i < ledsCount; i++)
    {
        leds[i][0] = from[i][0] * effectTransitionWeight + to[i][0] * (1 - effectTransitionWeight);
        leds[i][1] = from[i][1] * effectTransitionWeight + to[i][1] * (1 - effectTransitionWeight);
        leds[i][2] = from[i][2] * effectTransitionWeight + to[i][2] * (1 - effectTransitionWeight);
    }

    return transitionIsEndFlag;
}

void EController::SendColors(CRGB *ledsColors)
{
    for (int i = 0; i < ledsCount; i++)
    {
        leds[i] = ledsColors[i];
    }
}

void EController::SwitchEffect(byte effectId, bool implaceEffect, uint32_t *effectParams) {
    if (implaceEffect)
    {
        if (transitionDirectionToFirstEffect == true)
        {
            if (firstEffect != NULL)
            {
                delete firstEffect;
            }
            currentLEDBuffer = firstLEDBuffer;
            firstEffect = GetEffectById(effectId, firstLEDBuffer, ledsCount, effectParams);
            transitionDirectionToFirstEffect = true;
        }
        else
        {
            if (secondEffect != NULL)
            {
                delete secondEffect;
            }
            currentLEDBuffer = secondLEDBuffer;
            secondEffect = GetEffectById(effectId, secondLEDBuffer, ledsCount, effectParams);
            transitionDirectionToFirstEffect = false;
        }
        return;
    }
    if (firstEffect == NULL)
    {
        currentLEDBuffer = firstLEDBuffer;
        firstEffect = GetEffectById(effectId, firstLEDBuffer, ledsCount, effectParams);
        transitionDirectionToFirstEffect = true;
        effectTransitionStartTimestamp = millis();
        isTransitionRunning = true;
    }
    else if (secondEffect == NULL)
    {
        currentLEDBuffer = secondLEDBuffer;
        secondEffect = GetEffectById(effectId, secondLEDBuffer, ledsCount, effectParams);
        transitionDirectionToFirstEffect = false;
        effectTransitionStartTimestamp = millis();
        isTransitionRunning = true;
    }
    else
    {
        queuedEffectId = effectId;
        queuedEffectParams = effectParams;
    }
}