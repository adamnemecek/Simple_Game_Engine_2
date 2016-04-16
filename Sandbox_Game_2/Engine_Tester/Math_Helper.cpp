#include "Math_Helper.h"

namespace Math_Helper
{
   bool my_float_eq(const float expected, const float actual)
   {
      static const float ACCEPTABLE_ERROR = 0.00001f;

      float diff = fabs(expected - actual);
      if (diff > ACCEPTABLE_ERROR)
      {
         return false;
      }
      else
      {
         return true;
      }
   }
}

