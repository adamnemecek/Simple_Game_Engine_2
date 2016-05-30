#pragma once

// Marsaglia's xorshf generator
// According to http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c, 
// unless I'm doing cryptography, this generator rocks.

static unsigned long x = 123456789, y = 362436069, z = 521288629;

namespace Utilities
{
    //unsigned long xorshf96(void) {          //period 2^96-1
    unsigned long FastRandPosOnly(void) {          //period 2^96-1
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }

    // this is my crude adaptation that produces negative values too
    // Note: I don't know if substituting unsigned long for long messes up the randomness,
    // but it seems pretty good for making a random particle vector.
    long FastRandPosAndNeg(void) {          //period 2^96-1
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }

}
