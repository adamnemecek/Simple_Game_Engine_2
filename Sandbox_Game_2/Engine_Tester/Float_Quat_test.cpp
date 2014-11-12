// for the Google Test frame
#include <gtest\gtest.h>

// for the class under test
#include <Engine_2\Math\F_Quat.h>
using Math::F_Quat;

#include <math.h>

// a helper namespace for...helping things
namespace
{
   //void v(const glm::vec3 &V, const float F)
   //{
   //   EXPECT_FLOAT_EQ(0.0f, F);
   //   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.x);
   //   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.y);
   //   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.z);
   //}

   static const float PI = 3.141592653f;
   static const float PI_over_2 = PI / 2.0f;
   static const float PI_over_4 = PI / 4.0f;
   
   static const float cos_PI = cosf(PI);
   static const float cos_PI_over_2 = cosf(PI_over_2);
   static const float cos_PI_over_4 = cosf(PI_over_4);

   static const float sin_PI = sinf(PI);
   static const float sin_PI_over_2 = sinf(PI_over_2);
   static const float sin_PI_over_4 = sinf(PI_over_4);

   bool my_float_eq(const float expected, const float actual)
   {
      static const float ACCEPTABLE_ERROR = 0.00001f;

      float diff = abs(expected - actual);
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

TEST(F_Quat, Default_Constructor)
{
   F_Quat fq;
   EXPECT_FLOAT_EQ(0.0f, fq.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.z);
}

TEST(F_Quat, Explicit_Constructor)
{
   static const float SCALAR = 99.99f;
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat fq(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));
   EXPECT_FLOAT_EQ(SCALAR, fq.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X, fq.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y, fq.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z, fq.m_vector.z);
}

TEST(F_Quat, Real_Quat)
{
   static const float SCALAR = 99.99f;

   F_Quat fq = F_Quat::generate_real_quat(SCALAR);
   EXPECT_FLOAT_EQ(SCALAR, fq.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.z);
}

TEST(F_Quat, Pure_Quat)
{
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat fq = F_Quat::generate_pure_quat(glm::vec3(VEC_X, VEC_Y, VEC_Z));
   EXPECT_FLOAT_EQ(0.0f, fq.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X, fq.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y, fq.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z, fq.m_vector.z);
}

TEST(F_Quat, Rotator)
{
   // rotate 90 degrees around X
   glm::vec3 rotation_axis(+1.0f, 0.0f, 0.0f);
   float theta = PI_over_2;

   F_Quat fq = F_Quat::generate_rotator(rotation_axis, theta);
   //EXPECT_FLOAT_EQ(cosf(theta / 2.0f), fq.m_scalar);
   EXPECT_FLOAT_EQ(cosf(theta), fq.m_scalar);
   //EXPECT_FLOAT_EQ(sin(theta / 2.0f), fq.m_vector.x);
   EXPECT_FLOAT_EQ(sinf(theta), fq.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.z);

   // rotate 90 degrees around Y
   rotation_axis = glm::vec3(0.0f, +1.0f, 0.0f);

   fq = F_Quat::generate_rotator(rotation_axis, theta);
   //EXPECT_FLOAT_EQ(cos(theta / 2.0f), fq.m_scalar);
   EXPECT_FLOAT_EQ(cosf(theta), fq.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.x);
   //EXPECT_FLOAT_EQ(sin(theta / 2.0f), fq.m_vector.y);
   EXPECT_FLOAT_EQ(sinf(theta), fq.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.z);

   // rotate 90 degrees around Z
   rotation_axis = glm::vec3(0.0f, 0.0f, +1.0f);

   fq = F_Quat::generate_rotator(rotation_axis, theta);
   //EXPECT_FLOAT_EQ(cos(theta / 2.0f), fq.m_scalar);
   EXPECT_FLOAT_EQ(cosf(theta), fq.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, fq.m_vector.y);
   //EXPECT_FLOAT_EQ(sin(theta / 2.0f), fq.m_vector.z);
   EXPECT_FLOAT_EQ(sinf(theta), fq.m_vector.z);
}

TEST(F_Quat, Conjugate)
{
   static const float SCALAR = 99.99f;
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat fq(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));

   F_Quat conjugate = fq.conjugate();
   EXPECT_FLOAT_EQ(SCALAR, conjugate.m_scalar);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_X, conjugate.m_vector.x);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_Y, conjugate.m_vector.y);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_Z, conjugate.m_vector.z);
}

TEST(F_Quat, Magnitude)
{

}

TEST(F_Quat, Magnitude_Squared)
{

}

TEST(F_Quat, Normalize)
{

}

TEST(F_Quat, Inverse)
{

}

TEST(F_Quat, Addition)
{

}

TEST(F_Quat, Scalar_Multiplication)
{
   static const float V1_SCALAR = 1.0f;
   static const float V1_X = 1.1f;
   static const float V1_Y = 2.2f;
   static const float V1_Z = 3.3f;

   static const float V2_SCALAR = 1.0f;
   static const float V2_X = -3.3f;
   static const float V2_Y = -2.2f;
   static const float V2_Z = -1.1f;

   F_Quat fq1(V1_SCALAR, glm::vec3(V1_X, V1_Y, V1_Z));
   F_Quat fq2(V2_SCALAR, glm::vec3(V2_X, V2_Y, V2_Z));

   // two operator addition
   F_Quat result_1 = fq1 + fq2;
   EXPECT_FLOAT_EQ(V1_SCALAR + V2_SCALAR, result_1.m_scalar);
   EXPECT_FLOAT_EQ(V1_X + V2_X, result_1.m_vector.x);
   EXPECT_FLOAT_EQ(V1_Y + V2_Y, result_1.m_vector.y);
   EXPECT_FLOAT_EQ(V1_Z + V2_Z, result_1.m_vector.z);

   // self-addition
   fq1 += fq2;
   F_Quat result_2 = fq1;
   EXPECT_FLOAT_EQ(V1_SCALAR + V2_SCALAR, result_2.m_scalar);
   EXPECT_FLOAT_EQ(V1_X + V2_X, result_2.m_vector.x);
   EXPECT_FLOAT_EQ(V1_Y + V2_Y, result_2.m_vector.y);
   EXPECT_FLOAT_EQ(V1_Z + V2_Z, result_2.m_vector.z);
}

TEST(F_Quat, Multiplicaton)
{
   static const float V1_SCALAR = 1.0f;
   static const float V1_X = 1.1f;
   static const float V1_Y = 2.2f;
   static const float V1_Z = 3.3f;

   static const float V2_SCALAR = 1.0f;
   static const float V2_X = -3.3f;
   static const float V2_Y = -2.2f;
   static const float V2_Z = -1.1f;

   F_Quat fq1(1.0f, glm::vec3(1.1f, 2.2f, 3.3f));
   F_Quat fq2(2.0f, glm::vec3(-3.3, -2.2, -1.1));

   // I made the calculations in a notebook with assistence from glm's dot(...) and cross(...)
   F_Quat result_1 = fq1 * fq2;
   EXPECT_FLOAT_EQ(+14.1f, result_1.m_scalar);
   EXPECT_FLOAT_EQ(3.74f, result_1.m_vector.x);
   EXPECT_FLOAT_EQ(-7.48f, result_1.m_vector.y);
   EXPECT_FLOAT_EQ(10.34f, result_1.m_vector.z);

   fq1 *= fq2;
   F_Quat result_2 = fq1;
   EXPECT_FLOAT_EQ(+14.1f, result_2.m_scalar);
   EXPECT_FLOAT_EQ(3.74f, result_2.m_vector.x);
   EXPECT_FLOAT_EQ(-7.48f, result_2.m_vector.y);
   EXPECT_FLOAT_EQ(10.34f, result_2.m_vector.z);
}

TEST(F_Quat, Rotate_Points_Around_X)
{
   F_Quat point = F_Quat::generate_pure_quat(glm::vec3(0.0f, 1.0f, 0.0f));
   F_Quat rotator = F_Quat::generate_rotator(glm::vec3(1.0f, 0.0f, 0.0f), PI_over_2);

   // transform the point
   F_Quat result = point * rotator;
   EXPECT_FLOAT_EQ(0.0f, result.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, result.m_vector.x);
   //EXPECT_FLOAT_EQ(0, result.m_vector.y);
   EXPECT_TRUE(my_float_eq(0.0f, result.m_vector.y));
   EXPECT_FLOAT_EQ(-1.0f, result.m_vector.z);
}

TEST(F_Quat, Rotate_Points_Around_Y)
{

}

TEST(F_Quat, Rotate_Points_Around_Z)
{

}

TEST(F_Quat, Rotate_Points_Around_Arbitrary_Axis)
{

}
