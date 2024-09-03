#ifndef EFFECTTYPES_H
#define EFFECTTYPES_H

#include <cstdint>

struct DoubleCRGB;

struct DoubleCRGB
{
    union
    {
        struct
        {
            union
            {
                double r;
            };
            union
            {
                double g;
            };
            union
            {
                double b;
            };
        };

        double raw[3];
    };

    inline double &operator[](uint8_t x) __attribute__((always_inline))
    {
        assert(x < 3);
        return raw[x];
    }

    inline DoubleCRGB() __attribute__((always_inline)) = default;

    constexpr DoubleCRGB(double r, double g, double b) __attribute__((always_inline))
        : r(r), g(g), b(b)
    {
    }

    /// Allow assignment from one RGB struct to another
    inline DoubleCRGB& operator= (const DoubleCRGB& rhs) __attribute__((always_inline)) = default;
};

#endif // EFFECTTYPES_H