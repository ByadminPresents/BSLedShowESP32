#ifndef EFFECTS_H
#define EFFECTS_H

// Include here your effect
#include "Effect.h"
#include "SolidColorEffect.h"
#include "RainbowEffect.h"
#include "AuroraEffect.h"
#include "LightsReadyEffect.h"
#include "BeatSaberMainMenuEffect.h"
#include "ColoredStripsEffect.h"
//

static Effect* GetEffectById(byte effectId, CRGB *LEDBuffer, size_t ledsCount, uint32_t *effectParams);

// Assign an ID to your effect here
static Effect* GetEffectById(byte effectId, CRGB *LEDBuffer, size_t ledsCount, uint32_t *effectParams) {
    switch (effectId)
    {
    case 0:
        return new SolidColorEffect(LEDBuffer, ledsCount, effectParams);
    case 1:
        return new RainbowEffect(LEDBuffer, ledsCount, effectParams);
    case 2:
        return new AuroraEffect(LEDBuffer, ledsCount, effectParams);
    case 3:
        return new ColoredStripsEffect(LEDBuffer, ledsCount, effectParams);
    case 254:
        return new BeatSaberMainMenuEffect(LEDBuffer, ledsCount, effectParams);
    case 255:
        return new LightsReadyEffect(LEDBuffer, ledsCount, effectParams);
    default:
        break;
    }
}
//

#endif // EFFECTS_H