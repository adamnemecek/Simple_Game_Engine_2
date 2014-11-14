// for the Google Test frame
#include <gtest\gtest.h>

#include <Engine_2\Math\F_Dual_Quat.h>
using Math::F_Dual_Quat;

#include <Engine_2\Math\F_Quat.h>
using Math::F_Quat;

#include <iostream>
using std::cout;
using std::endl;

// this file contains some useful constants to save time and keep magic numbers constant during testing
#include "Math_Helper.h"


TEST(Float_Dual_Quat, Default_Constructor)
{
   F_Dual_Quat dq;

   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(0.0f, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, dq.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Explicit_Constructor)
{
   F_Quat q1(1.0f, glm::vec3(+1.1f, +2.2f, +3.3f));
   F_Quat q2(2.0f, glm::vec3(-3.3f, -2.2f, -1.1f));
   F_Dual_Quat dq(q1, q2);

   EXPECT_FLOAT_EQ(+1.0f, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(+1.1f, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(+2.2f, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(+3.3f, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(+2.0f, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(-3.3f, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(-2.2f, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(-1.1f, dq.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Convenience_Constructor)
{
   static const float ROTATE_X = 1.1f;
   static const float ROTATE_Y = 2.2f;
   static const float ROTATE_Z = 3.3f;
   glm::vec3 rotation_axis(ROTATE_X, ROTATE_Y, ROTATE_Z);

   static const float ROT_ANGLE = Math_Helper::PI_over_2;

   static const float TRANSLATE_X = -3.3f;
   static const float TRANSLATE_Y = -2.2f;
   static const float TRANSLATE_Z = -1.1f;
   glm::vec3 translation(TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z);

   F_Dual_Quat dq(rotation_axis, ROT_ANGLE, translation);
   F_Quat expected_real = F_Quat::generate_rotator(rotation_axis, ROT_ANGLE);
   F_Quat expected_dual = F_Quat::generate_pure_quat(0.5f * translation) * expected_real;

   EXPECT_FLOAT_EQ(expected_real.m_scalar, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(expected_real.m_vector.x, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(expected_real.m_vector.y, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(expected_real.m_vector.z, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(expected_dual.m_scalar, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.x, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.y, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.z, dq.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Generate_Translate)
{
   static const float TRANSLATE_X = -3.3f;
   static const float TRANSLATE_Y = -2.2f;
   static const float TRANSLATE_Z = -1.1f;

   F_Dual_Quat dq = F_Dual_Quat::generate_translate_only(glm::vec3(TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z));
   EXPECT_FLOAT_EQ(1.0f, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(0.0f, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ((0.5f) * TRANSLATE_X, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ((0.5f) * TRANSLATE_Y, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ((0.5f) * TRANSLATE_Z, dq.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Generate_Orientation)
{
   static const float ROTATE_X = 1.1f;
   static const float ROTATE_Y = 2.2f;
   static const float ROTATE_Z = 3.3f;
   glm::vec3 rotation_axis(ROTATE_X, ROTATE_Y, ROTATE_Z);

   static const float ROT_ANGLE = 1.0f;

   F_Dual_Quat dq = F_Dual_Quat::generate_rotator_only(rotation_axis, ROT_ANGLE);
   F_Quat expected_real = F_Quat::generate_rotator(rotation_axis, ROT_ANGLE);
   F_Quat expected_dual = F_Quat::generate_pure_quat(glm::vec3()) * expected_real;

   EXPECT_FLOAT_EQ(expected_real.m_scalar, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(expected_real.m_vector.x, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(expected_real.m_vector.y, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(expected_real.m_vector.z, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(expected_dual.m_scalar, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.x, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.y, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(expected_dual.m_vector.z, dq.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Normalize)
{
   F_Quat real(1.0f, glm::vec3(2.0f, 3.0f, 4.0f));
   F_Quat dual(5.0f, glm::vec3(6.0f, 7.0f, 8.0f));
   F_Dual_Quat dq(real, dual);

   // I solved this by hand and got some help with C++ for exact float multiplication,
   // so I am using magic numbers here.
   dq.normalize();
   EXPECT_FLOAT_EQ(+0.18257418f, dq.m_real.m_scalar);
   EXPECT_FLOAT_EQ(+0.36514837f, dq.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(+0.54772258f, dq.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(+0.73029673f, dq.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(+0.48686451f, dq.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(+0.2434324f, dq.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(+2.3841858e-007f, dq.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(-0.24343204f, dq.m_dual.m_vector.z);

   // multiply the normalized form by its conjugate to see the result is 1 
   F_Dual_Quat dq_conjugate = dq.conjugate();
   F_Dual_Quat result = dq * dq_conjugate;
   EXPECT_FLOAT_EQ(1.0f, result.m_real.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, result.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, result.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, result.m_real.m_vector.z);
   EXPECT_TRUE(Math_Helper::my_float_eq(0.0f, result.m_dual.m_scalar));
   EXPECT_FLOAT_EQ(0.0f, result.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, result.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, result.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Conjugate)
{
   F_Quat real(1.0f, glm::vec3(1.1f, 2.2f, 3.3f));
   F_Quat dual(0.0f, glm::vec3(4.4f, 5.5f, 6.6f));
   F_Dual_Quat dq(real, dual);

   F_Dual_Quat conjugate = dq.conjugate();

   EXPECT_FLOAT_EQ(+1.0f, conjugate.m_real.m_scalar);
   EXPECT_FLOAT_EQ(-1.1f, conjugate.m_real.m_vector.x);
   EXPECT_FLOAT_EQ(-2.2f, conjugate.m_real.m_vector.y);
   EXPECT_FLOAT_EQ(-3.3f, conjugate.m_real.m_vector.z);
   EXPECT_FLOAT_EQ(+0.0f, conjugate.m_dual.m_scalar);
   EXPECT_FLOAT_EQ(-4.4f, conjugate.m_dual.m_vector.x);
   EXPECT_FLOAT_EQ(-5.5f, conjugate.m_dual.m_vector.y);
   EXPECT_FLOAT_EQ(-6.6f, conjugate.m_dual.m_vector.z);
}

TEST(Float_Dual_Quat, Multiply)
{
   F_Quat real_1(1.0f, glm::vec3(1.1f, 2.2f, 3.3f));
   F_Quat dual_1(0.0f, glm::vec3(4.4f, 5.5f, 6.6f));
   F_Dual_Quat dq1(real_1, dual_1);

   F_Quat real_2(1.0f, glm::vec3(-3.3f, -2.2f, -1.1f));
   F_Quat dual_2(0.0f, glm::vec3(-6.6f, -5.5f, -4.4f));
   F_Dual_Quat dq2(real_2, dual_2);

   // put in its own block to avoid "result_X" mixups
   {
      F_Dual_Quat result_1 = dq1 * dq2;
      EXPECT_FLOAT_EQ(+13.1f, result_1.m_real.m_scalar);
      EXPECT_FLOAT_EQ(+2.64f, result_1.m_real.m_vector.x);
      EXPECT_FLOAT_EQ(-9.68f, result_1.m_real.m_vector.y);
      EXPECT_FLOAT_EQ(+7.04f, result_1.m_real.m_vector.z);
      EXPECT_FLOAT_EQ(+67.76f, result_1.m_dual.m_scalar);
      EXPECT_FLOAT_EQ(+14.74f, result_1.m_dual.m_vector.x);
      EXPECT_FLOAT_EQ(-33.88f, result_1.m_dual.m_vector.y);
      EXPECT_FLOAT_EQ(+19.14f, result_1.m_dual.m_vector.z);
   }

   {
      dq1 *= dq2;
      F_Dual_Quat result_2 = dq1;
      EXPECT_FLOAT_EQ(+13.1f,  result_2.m_real.m_scalar);
      EXPECT_FLOAT_EQ(+2.64f,  result_2.m_real.m_vector.x);
      EXPECT_FLOAT_EQ(-9.68f,  result_2.m_real.m_vector.y);
      EXPECT_FLOAT_EQ(+7.04f,  result_2.m_real.m_vector.z);
      EXPECT_FLOAT_EQ(+67.76f, result_2.m_dual.m_scalar);
      EXPECT_FLOAT_EQ(+14.74f, result_2.m_dual.m_vector.x);
      EXPECT_FLOAT_EQ(-33.88f, result_2.m_dual.m_vector.y);
      EXPECT_FLOAT_EQ(+19.14f, result_2.m_dual.m_vector.z);
   }
}

TEST(Float_Dual_Quat, Multiply_Quat_by_Dual_Quat)
{

}


TEST(Float_Dual_Quat, Translate_Point)
{

}

