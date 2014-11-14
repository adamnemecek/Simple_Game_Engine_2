#ifndef ENGINE_TESTER_MATH_HELPER_H
#define ENGINE_TESTER_MATH_HELPER_H

//void v(const glm::vec3 &V, const float F)
//{
//   EXPECT_FLOAT_EQ(0.0f, F);
//   EXPECT_FLOAT_EQ(0.0f, q.m_vector.x);
//   EXPECT_FLOAT_EQ(0.0f, q.m_vector.y);
//   EXPECT_FLOAT_EQ(0.0f, q.m_vector.z);
//}

#include <math.h>

namespace Math_Helper
{
   static const float PI = 3.141592653f;
   static const float PI_over_2 = PI / 2.0f;
   static const float PI_over_4 = PI / 4.0f;

   static const float cos_PI = cosf(PI);
   static const float cos_PI_over_2 = cosf(PI_over_2);
   static const float cos_PI_over_4 = cosf(PI_over_4);

   static const float sin_PI = sinf(PI);
   static const float sin_PI_over_2 = sinf(PI_over_2);
   static const float sin_PI_over_4 = sinf(PI_over_4);

   bool my_float_eq(const float expected, const float actual);
}


#endif
