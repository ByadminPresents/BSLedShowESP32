#ifndef EFFECTTYPES_H
#define EFFECTTYPES_H

#include <cstdint>

struct FloatCRGB;

struct FloatCRGB
{
    union
    {
        struct
        {
            union
            {
                float r;
            };
            union
            {
                float g;
            };
            union
            {
                float b;
            };
        };

        float raw[3];
    };

    inline float &operator[](uint8_t x) __attribute__((always_inline))
    {
        assert(x < 3);
        return raw[x];
    }

    inline FloatCRGB() __attribute__((always_inline)) = default;

    constexpr FloatCRGB(float r, float g, float b) __attribute__((always_inline))
        : r(r), g(g), b(b)
    {
    }

    /// Allow assignment from one RGB struct to another
    inline FloatCRGB& operator= (const FloatCRGB& rhs) __attribute__((always_inline)) = default;
};

#endif // EFFECTTYPES_H