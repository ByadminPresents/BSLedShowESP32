#include "AuroraEffect.h"
#include <EffectUtilFunctions.h>

AuroraEffect::AuroraEffect(CRGB *leds, uint16_t ledsCount, uint32_t *params) : valuesCount(ledsCount), Effect(leds, ledsCount, params)
{
    maxCountOfProjectilesPerNode = params[1];
    maxCountOfNodes = params[0];
}

void AuroraEffect::PreIterate(int preIterationsCount)
{
    preIterationsLeft = preIterationsCount;
    while(preIterationsLeft > 0)
    {
        AuroraEffect::Iterate();
    }
}

bool AuroraEffect::Iterate()
{
    if (!isVectorsAllocated)
    {
        nodes.reserve(maxCountOfNodes);
   
        for (int i = 0; i < maxCountOfNodes; i++)
        {
            nodes.emplace_back(ledsCount, maxCountOfProjectilesPerNode);
        }
        isVectorsAllocated = true;
    }

    bool initAuroraNodeFlag = currentCountOfNodes != maxCountOfNodes && GetRandomValue(0, 100) == 0;
    currentCountOfNodes = 0;
    for (int i = 0; i < maxCountOfNodes; i++)
    {
        if (!nodes[i].Iterate())
        {
            if (initAuroraNodeFlag)
            {
                initAuroraNodeFlag = false;
                nodes[i].Init(GetRandomValue(0, valuesCount));
            }
        }
        else
        {
            currentCountOfNodes++;
        }
    }

    // if (preIterationsLeft > 0)
    // {
    //     preIterationsLeft--;
    //     return true;
    // }

    for (int i = 0; i < ledsCount; i++)
    {
        DoubleCRGB resultColor = DoubleCRGB(AuroraEffect::defaultR, AuroraEffect::defaultG, AuroraEffect::defaultB);

        for (int j = 0; j < maxCountOfNodes; j++)
        {
            DoubleCRGB tempColor = nodes[j].GetColor(i);

            if (tempColor.r == AuroraEffect::defaultR && tempColor.g == AuroraEffect::defaultG && tempColor.b == AuroraEffect::defaultB)
            {
                continue;
            }

            for (int c = 0; c < 3; c++)
            {
                resultColor[c] = max(resultColor[c], tempColor[c]);
            }
        }

        for (int c = 0; c < 3; c++)
        {
            leds[i][c] = leds[i][c] + (resultColor[c] - leds[i][c]) / 5;
        }

    }

    // CRGB prevColor = leds[-1 % valuesCount];

    // for (int i = 0; i < ledsCount; i++)
    // {
    //     CRGB currentColor = leds[i % valuesCount];
    //     for (int c = 0; c < 3; c++)
    //     {
    //         leds[i % valuesCount][c] = (prevColor[c] + currentColor[c] + leds[(i + 1) % valuesCount][c]) / 3;
    //     }
    //     prevColor = currentColor;
    // }

    return true;
}

bool AuroraEffect::TransitionIterate()
{
    if (!isVectorsAllocated)
    {
        for (int i = 0; i < valuesCount; i++)
        {
            leds[i].r = AuroraEffect::defaultR;
            leds[i].g = AuroraEffect::defaultG; 
            leds[i].b = AuroraEffect::defaultB;
        }
    }
    else
    {
        AuroraEffect::Iterate();
    }  
    return true;
}

bool AuroraEffect::AuroraNode::Iterate()
{
    AuroraNode::positionOffset += AuroraNode::positionOffsetSpeed;
    if (AuroraNode::positionOffset < 0)
    {
        AuroraNode::positionOffset += valuesCount;
    }
    else if (AuroraNode::positionOffset > valuesCount)
    {
        AuroraNode::positionOffset -= valuesCount;
    }

    if (AuroraNode::isInit)
    {
        if (AuroraNode::isBoosted)
        {
            AuroraNode::maxProjectilePowerValue += GetRandomValue(-0.03, 0.01);
        }
        else
        {
            AuroraNode::maxProjectilePowerValue += GetRandomValue(-0.01, 0.08);
            if (AuroraNode::maxProjectilePowerValue < 0.05)
            {
                AuroraNode::maxProjectilePowerValue = 0.05;
            }
            if (AuroraNode::maxProjectilePowerValue >= AuroraEffect::powerBoostThreshold)
            {
                AuroraNode::isBoosted = true;
            }
        }
    }

    if (AuroraNode::maxProjectilePowerValue <= 0.5)
    {
        AuroraNode::maxProjectilePowerValue = 0;
        AuroraNode::isInit = false;
    }
    else if (AuroraNode::maxProjectilePowerValue > AuroraEffect::maxPowerThreshold)
    {
        AuroraNode::maxProjectilePowerValue = AuroraEffect::maxPowerThreshold;
    }

    bool initProjectilesFlag = isInit && currentCountOfProjectiles != maxCountOfProjectiles && GetRandomValue(0, currentCountOfProjectiles + 1) == 0;
    currentCountOfProjectiles = 0;
    for (int i = 0; i < maxCountOfProjectiles; i++)
    {
        if (!projectiles[i].Iterate(doubleLEDValues, valuesCount))
        {
            if (initProjectilesFlag)
            {
                initProjectilesFlag = false;
                projectiles[i].Init(maxProjectilePowerValue, projectilesSpawnPosition, projectilesSpawnDispersion);
            }
        }
        else
        {
            currentCountOfProjectiles++;
        }
    }

    for (int i = 0; i < valuesCount; i++)
    {
        if (doubleLEDValues[i][0] > defaultR)
        {
            doubleLEDValues[i][0] -= (doubleLEDValues[i][0] - defaultR) / 20;
        }
        
        if (doubleLEDValues[i][1] > defaultG)
        {
            doubleLEDValues[i][1] -= doubleLEDValues[i][1] / 500;
        }
        
        if (doubleLEDValues[i][2] > defaultB)
        {
            doubleLEDValues[i][2] -= pow(doubleLEDValues[i][2], 2) / 1000;
        }

        if (doubleLEDValues[i][0] < defaultR)
        {
            doubleLEDValues[i][0] = defaultR;
        }
                
        if (doubleLEDValues[i][1] < defaultG)
        {
            doubleLEDValues[i][1] = defaultG;
        }
                
        if (doubleLEDValues[i][2] < defaultB)
        {
            doubleLEDValues[i][2] = defaultB;
        }        
    }

    projectilesSpawnPosition += GetRandomValue(-2.0, 2.0);
    projectilesSpawnDispersion += GetRandomValue(-2.0, 2.0);
    if (projectilesSpawnDispersion < 80)
    {
        projectilesSpawnDispersion = 80;
    }
    else if (projectilesSpawnDispersion > valuesCount / 4)
    {
        projectilesSpawnDispersion = valuesCount / 4;
    }

    return isInit;
}

AuroraEffect::AuroraNode::AuroraNode(int valuesCount, int maxCountOfProjectiles) : valuesCount(valuesCount), maxCountOfProjectiles(maxCountOfProjectiles)
{
    doubleLEDValues.reserve(valuesCount);
    projectiles.reserve(maxCountOfProjectiles);

    for (int i = 0; i < valuesCount; i++)
    {
        doubleLEDValues.emplace_back((double)defaultR, (double)defaultG, (double)defaultB);
    }

    for (int i = 0; i < maxCountOfProjectiles; i++)
    {
        projectiles.emplace_back();
    }
}


void AuroraEffect::AuroraNode::Init(int projectilesSpawnPosition)
{
    AuroraNode::isBoosted = false;
    AuroraNode::projectilesSpawnPosition = projectilesSpawnPosition;
    AuroraNode::projectilesSpawnDispersion = GetRandomValue(80.0, valuesCount / 6.0);
    AuroraNode::maxProjectilePowerValue = GetRandomValue(0.5, 1.0);
    AuroraNode::positionOffsetSpeed = GetRandomValue(0.005, 0.1);
    if (GetRandomValue(0, 2) == 0)
    {
        AuroraNode::positionOffsetSpeed *= -1;
    }

    AuroraNode::isInit = true;
}

DoubleCRGB AuroraEffect::AuroraNode::GetColor(int index)
{
    DoubleCRGB color;

    index = (index + (int)floor(positionOffset)) % valuesCount;
    if (index < 0)
    {
        index = valuesCount + index;
    }

    color.r = doubleLEDValues[index][0] * (1 - abs((abs(positionOffset) - floor(positionOffset)))) + doubleLEDValues[(index + 1) % valuesCount][0] * abs((abs(positionOffset) - floor(positionOffset)));
    color.g = doubleLEDValues[index][1] * (1 - abs((abs(positionOffset) - floor(positionOffset)))) + doubleLEDValues[(index + 1) % valuesCount][1] * abs((abs(positionOffset) - floor(positionOffset)));
    color.b = doubleLEDValues[index][2] * (1 - abs((abs(positionOffset) - floor(positionOffset)))) + doubleLEDValues[(index + 1) % valuesCount][2] * abs((abs(positionOffset) - floor(positionOffset)));

    return color;
}

AuroraEffect::AuroraNode::AuroraProjectile::AuroraProjectile()
{
    power = 0;
}

void AuroraEffect::AuroraNode::AuroraProjectile::Init(double maxPower, int x, int dispersion)
{
    AuroraProjectile::x = GetRandomValue(x - dispersion, x + dispersion);
    power = GetRandomValue(maxPower * (1 / (10 + abs(x - AuroraProjectile::x) / 16)), maxPower * (1 / (1 + abs(x - AuroraProjectile::x) / 16)));
    maxPowerLoss = power / 10;
    minPowerLoss = power / 100;
    speed = GetRandomValue(0.3, 1.5);

    if (GetRandomValue(-10000, 10000) == 0)
    {
        AuroraProjectile::x = GetRandomValue(x - dispersion * 3, x + dispersion * 3);
        power = 200;
        maxPowerLoss = power / 40;
        minPowerLoss = power / 130;
        speed = 2.5;
    }

    if (GetRandomValue(0, 2) == 0)
    {
        speed *= -1;
    }
}

void AuroraEffect::AuroraNode::AuroraProjectile::ApplyColorForX(int x, std::vector<DoubleCRGB>& values, int valuesCount)
{
    for (int i = -3; i <= 3; i++)
    {
        int index = (x + i) % valuesCount;
        if (index < 0)
        {
            index = (valuesCount + index) % valuesCount;
        }

        values[index][1] += (1 / abs(speed)) * power * (1 / (abs(i) + 1));
        if (values[index][1] > 255)
        {
            values[index][1] = 255;
        }
        values[index][0] += (values[index][1] - values[index][0]) / (3 + abs(i) + 1);
        values[index][2] += (values[index][1] - values[index][2]) / (2 + abs(i) + 1);
        if (values[index][0] > 255)
        {
            values[index][0] = 255;
        }
        if (values[index][2] > 255)
        {
            values[index][2] = 255;
        }
    }
    
}

bool AuroraEffect::AuroraNode::AuroraProjectile::Iterate(std::vector<DoubleCRGB>& values, int valuesCount)//std::vector<DoubleCRGB>& values
{
    if (power <= 0.01)
    {
        power = 0;
        return false;
    }

    double x1 = x, x2 = x;
    x += speed;
    if (speed < 0)
    {
        x1 += speed;
    }
    else
    {
        x2 += speed;
    }

    for (int i = round(x1); i <= round(x2); i++)
    {
        ApplyColorForX(i, values, valuesCount);
    }

    power = round((power - GetRandomValue(minPowerLoss, maxPowerLoss)) * 1000) / 1000;

    if (power <= 0.01)
    {
        power = 0;
        return false;
    }
    return true;
}