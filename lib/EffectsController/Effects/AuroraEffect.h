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

        std::vector<DoubleCRGB> doubleLEDValues;
        int valuesCount;

        double positionOffset = 0;
        double positionOffsetSpeed = 0;

        double projectilesSpawnPosition = 0;
        double projectilesSpawnDispersion = 0;

        double maxProjectilePowerValue = 0;


        std::vector<AuroraProjectile> projectiles;
        int maxCountOfProjectiles;
        int currentCountOfProjectiles = 0;

        class AuroraProjectile
        {
        private:
            double x;
            double speed;
            double power = 0;
            double maxPowerLoss;
            double minPowerLoss;

            void ApplyColorForX(int x, std::vector<DoubleCRGB>& values, int valuesCount);

        public:
            AuroraProjectile();

            void Init(double maxPower, int x, int dispersion);

            bool Iterate(std::vector<DoubleCRGB>& values, int valuesCount);
        };

    public:
        AuroraNode() {}

        AuroraNode(int valuesCount, int maxCountOfProjectiles);

        ~AuroraNode() {}

        void Init(int x);

        bool Iterate();

        DoubleCRGB GetColor(int index);
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