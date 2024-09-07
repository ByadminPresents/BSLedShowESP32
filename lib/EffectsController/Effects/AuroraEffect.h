#ifndef AURORAEFFECT_H
#define AURORAEFFECT_H

#include "Effect.h"
#include <FastLED.h>
#include <EffectTypes.h>
#include <vector>

class AuroraEffect : public Effect
{
private:
    class AuroraNode;

    bool isVectorsAllocated = false;

    int preIterationsLeft = 0;

    int maxCountOfNodes = 3;
    int maxCountOfProjectilesPerNode = 10;
    int currentCountOfNodes = 0;
    int valuesCount;
    std::vector<AuroraNode> nodes;

    class AuroraNode
    {
    private:
        bool isBoosted = false;

        bool isInit = false;

        struct AuroraProjectile;

        std::vector<FloatCRGB> floatLEDValues;
        int valuesCount;

        float positionOffset = 0;
        float positionOffsetSpeed = 0;

        float projectilesSpawnPosition = 0;
        float projectilesSpawnDispersion = 0;

        float maxProjectilePowerValue = 0;


        std::vector<AuroraProjectile> projectiles;
        int maxCountOfProjectiles;
        int currentCountOfProjectiles = 0;

        class AuroraProjectile
        {
        private:
            float x;
            float speed;
            float power = 0;
            float maxPowerLoss;
            float minPowerLoss;

            void ApplyColorForX(int x, std::vector<FloatCRGB>& values, int valuesCount);

        public:
            AuroraProjectile();

            void Init(float maxPower, int x, int dispersion);

            bool Iterate(std::vector<FloatCRGB>& values, int valuesCount);
        };

    public:
        AuroraNode() {}

        AuroraNode(int valuesCount, int maxCountOfProjectiles);

        ~AuroraNode() {}

        void Init(int x);

        bool Iterate();

        FloatCRGB GetColor(int index);
    };

public:
    static const int defaultR = 1, defaultG = 2, defaultB = 8, powerBoostThreshold = 15, maxPowerThreshold = 25;

    AuroraEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params);

    ~AuroraEffect() override {}

    bool Iterate() override;

    bool TransitionIterate() override;

    void PreIterate(int preIterationsCount);
};


#endif // RAINBOWEFFECT_H