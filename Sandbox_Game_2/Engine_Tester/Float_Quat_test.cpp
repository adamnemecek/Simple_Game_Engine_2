// for the Google Test frame
#include <gtest\gtest.h>

// for the class under test
#include <Engine_2\Math\F_Quat.h>
using Math::F_Quat;

// this file contains some useful constants to save time and keep magic numbers constant during testing
#include "Math_Helper.h"

TEST(F_Quat, Default_Constructor)
{
   F_Quat q;
   EXPECT_FLOAT_EQ(0.0f, q.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.z);
}

TEST(F_Quat, Explicit_Constructor)
{
   static const float SCALAR = 99.99f;
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat q(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));
   EXPECT_FLOAT_EQ(SCALAR, q.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X, q.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y, q.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z, q.m_vector.z);
}

TEST(F_Quat, Real_Quat)
{
   static const float SCALAR = 99.99f;

   F_Quat q = F_Quat::generate_real_quat(SCALAR);
   EXPECT_FLOAT_EQ(SCALAR, q.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.z);
}

TEST(F_Quat, Pure_Quat)
{
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat q = F_Quat::generate_pure_quat(glm::vec3(VEC_X, VEC_Y, VEC_Z));
   EXPECT_FLOAT_EQ(0.0f, q.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X, q.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y, q.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z, q.m_vector.z);
}

TEST(F_Quat, Rotator)
{
   // rotate 90 degrees around X
   glm::vec3 rotation_vector(+1.0f, 0.0f, 0.0f);
   float theta = Math_Helper::PI_over_2;

   F_Quat q = F_Quat::generate_rotator(rotation_vector, theta);
   EXPECT_FLOAT_EQ(cosf(theta / 2.0f), q.m_scalar);
   EXPECT_FLOAT_EQ(sin(theta / 2.0f), q.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.z);

   // rotate 90 degrees around Y
   rotation_vector = glm::vec3(0.0f, +1.0f, 0.0f);

   q = F_Quat::generate_rotator(rotation_vector, theta);
   EXPECT_FLOAT_EQ(cos(theta / 2.0f), q.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.x);
   EXPECT_FLOAT_EQ(sin(theta / 2.0f), q.m_vector.y);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.z);

   // rotate 90 degrees around Z
   rotation_vector = glm::vec3(0.0f, 0.0f, +1.0f);

   q = F_Quat::generate_rotator(rotation_vector, theta);
   EXPECT_FLOAT_EQ(cos(theta / 2.0f), q.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.x);
   EXPECT_FLOAT_EQ(0.0f, q.m_vector.y);
   EXPECT_FLOAT_EQ(sin(theta / 2.0f), q.m_vector.z);
}

TEST(F_Quat, Self_Conjugate)
{
   static const float SCALAR = 99.99f;
   static const float VEC_X = 1.1f;
   static const float VEC_Y = 2.2f;
   static const float VEC_Z = 3.3f;

   F_Quat q(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));

   F_Quat conjugate = q.conjugate();
   EXPECT_FLOAT_EQ(SCALAR, conjugate.m_scalar);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_X, conjugate.m_vector.x);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_Y, conjugate.m_vector.y);
   EXPECT_FLOAT_EQ((-1.0f) * VEC_Z, conjugate.m_vector.z);
}

TEST(F_Quat, Self_Normalize)
{
   static const float SCALAR = 1.0f;
   static const float VEC_X = 4.0f;
   static const float VEC_Y = 4.0f;
   static const float VEC_Z = (-1.0f) * 4.0f;

   F_Quat q(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));

   q.normalize();

   // I solved this problem by hand, so I am using magic numbers here.
   // Note: I got the idea from 3dgep.com's article on quaternions.
   EXPECT_FLOAT_EQ(SCALAR / 7, q.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X / 7, q.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y / 7, q.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z / 7, q.m_vector.z);
}

TEST(F_Quat, Normalize)
{
   static const float SCALAR = 1.0f;
   static const float VEC_X = 4.0f;
   static const float VEC_Y = 4.0f;
   static const float VEC_Z = (-1.0f) * 4.0f;

   F_Quat q(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));

   F_Quat normalized_q = F_Quat::normalize(q);

   // I solved this problem by hand, so I am using magic numbers here.
   // Note: I got the idea from 3dgep.com's article on quaternions.
   EXPECT_FLOAT_EQ(SCALAR / 7, normalized_q.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X / 7, normalized_q.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y / 7, normalized_q.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z / 7, normalized_q.m_vector.z);
}

TEST(F_Quat, Addition)
{
   static const float V1_SCALAR = 1.0f;
   static const float V1_X = 1.1f;
   static const float V1_Y = 2.2f;
   static const float V1_Z = 3.3f;

   static const float V2_SCALAR = 1.0f;
   static const float V2_X = -3.3f;
   static const float V2_Y = -2.2f;
   static const float V2_Z = -1.1f;

   F_Quat q1(V1_SCALAR, glm::vec3(V1_X, V1_Y, V1_Z));
   F_Quat q2(V2_SCALAR, glm::vec3(V2_X, V2_Y, V2_Z));

   // two operator addition
   F_Quat result_1 = q1 + q2;
   EXPECT_FLOAT_EQ(V1_SCALAR + V2_SCALAR, result_1.m_scalar);
   EXPECT_FLOAT_EQ(V1_X + V2_X, result_1.m_vector.x);
   EXPECT_FLOAT_EQ(V1_Y + V2_Y, result_1.m_vector.y);
   EXPECT_FLOAT_EQ(V1_Z + V2_Z, result_1.m_vector.z);

   // self-addition
   q1 += q2;
   F_Quat result_2 = q1;
   EXPECT_FLOAT_EQ(V1_SCALAR + V2_SCALAR, result_2.m_scalar);
   EXPECT_FLOAT_EQ(V1_X + V2_X, result_2.m_vector.x);
   EXPECT_FLOAT_EQ(V1_Y + V2_Y, result_2.m_vector.y);
   EXPECT_FLOAT_EQ(V1_Z + V2_Z, result_2.m_vector.z);
}

TEST(F_Quat, Subtraction)
{
   static const float V1_SCALAR = 1.0f;
   static const float V1_X = 1.1f;
   static const float V1_Y = 2.2f;
   static const float V1_Z = 3.3f;

   static const float V2_SCALAR = 1.0f;
   static const float V2_X = -3.3f;
   static const float V2_Y = -2.2f;
   static const float V2_Z = -1.1f;

   F_Quat q1(V1_SCALAR, glm::vec3(V1_X, V1_Y, V1_Z));
   F_Quat q2(V2_SCALAR, glm::vec3(V2_X, V2_Y, V2_Z));

   // two operator addition
   F_Quat result_1 = q1 - q2;
   EXPECT_FLOAT_EQ(V1_SCALAR - V2_SCALAR, result_1.m_scalar);
   EXPECT_FLOAT_EQ(V1_X - V2_X, result_1.m_vector.x);
   EXPECT_FLOAT_EQ(V1_Y - V2_Y, result_1.m_vector.y);
   EXPECT_FLOAT_EQ(V1_Z - V2_Z, result_1.m_vector.z);
}

TEST(F_Quat, Scalar_Multiplication)
{
   static const float SCALAR = 1.0f;
   static const float VEC_X = 4.0f;
   static const float VEC_Y = 4.0f;
   static const float VEC_Z = (-1.0f) * 4.0f;

   F_Quat q(SCALAR, glm::vec3(VEC_X, VEC_Y, VEC_Z));

   float S = 2.2f;

   // test right multiply
   F_Quat result = q * S;

   EXPECT_FLOAT_EQ(SCALAR * S, result.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X * S, result.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y * S, result.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z * S, result.m_vector.z);

   // test left multiply
   result = S * q;
   EXPECT_FLOAT_EQ(SCALAR * S, result.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X * S, result.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y * S, result.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z * S, result.m_vector.z);

   // test self-multiply
   q *= S;
   result = q;
   EXPECT_FLOAT_EQ(SCALAR * S, result.m_scalar);
   EXPECT_FLOAT_EQ(VEC_X * S, result.m_vector.x);
   EXPECT_FLOAT_EQ(VEC_Y * S, result.m_vector.y);
   EXPECT_FLOAT_EQ(VEC_Z * S, result.m_vector.z);
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

   F_Quat q1(1.0f, glm::vec3(1.1f, 2.2f, 3.3f));
   F_Quat q2(2.0f, glm::vec3(-3.3, -2.2, -1.1));

   // I made the calculations in a notebook with assistence from glm's dot(...) and cross(...)
   F_Quat result_1 = q1 * q2;
   EXPECT_FLOAT_EQ(+14.1f, result_1.m_scalar);
   EXPECT_FLOAT_EQ(3.74f, result_1.m_vector.x);
   EXPECT_FLOAT_EQ(-7.48f, result_1.m_vector.y);
   EXPECT_FLOAT_EQ(10.34f, result_1.m_vector.z);

   q1 *= q2;
   F_Quat result_2 = q1;
   EXPECT_FLOAT_EQ(+14.1f, result_2.m_scalar);
   EXPECT_FLOAT_EQ(3.74f, result_2.m_vector.x);
   EXPECT_FLOAT_EQ(-7.48f, result_2.m_vector.y);
   EXPECT_FLOAT_EQ(10.34f, result_2.m_vector.z);
}

TEST(F_Quat, Rotate_Points_Around_X)
{
   F_Quat point = F_Quat::generate_pure_quat(glm::vec3(0.0f, 1.0f, 0.0f));
   F_Quat rotator = F_Quat::generate_rotator(glm::vec3(1.0f, 0.0f, 0.0f), Math_Helper::PI_over_2);

   // transform the point
   F_Quat result = rotator * point * rotator.conjugate();
   EXPECT_FLOAT_EQ(0.0f, result.m_scalar);
   EXPECT_FLOAT_EQ(0.0f, result.m_vector.x);
   //EXPECT_FLOAT_EQ(0, result.m_vector.y);
   EXPECT_TRUE(Math_Helper::my_float_eq(0.0f, result.m_vector.y));
   EXPECT_FLOAT_EQ(1.0f, result.m_vector.z);
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
