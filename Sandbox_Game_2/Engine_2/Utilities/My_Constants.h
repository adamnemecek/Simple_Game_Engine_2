#ifndef ENGINE_MY_CONSTANTS_H
#define ENGINE_MY_CONSTANTS_H

namespace Utilities
{
   static const float MY_PI = 3.14159f;
   static const float MY_HALF_PI = MY_PI / 2.0f;

   // because dividing by RAND_MAX for every particle is expensive
   static const float INVERSE_RAND_MAX = 1.0f / (float)RAND_MAX;

}

#endif