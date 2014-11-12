// for the Google Test frame
#include <gtest\gtest.h>

#include <Engine_2\Math\F_Dual_Quat.h>
using Math::F_Dual_Quat;

#include <Engine_2\Math\F_Quat.h>
using Math::F_Quat;

#include <iostream>
using std::cout;
using std::endl;


TEST(Float_Dual_Quat, Default_Constructor)
{

}

TEST(Float_Dual_Quat, Explicit_Constructor)
{

}

TEST(Float_Dual_Quat, Translate)
{

}

TEST(Float_Dual_Quat, Orientation)
{

}

TEST(Float_Dual_Quat, Magnitude)
{

}

TEST(Float_Dual_Quat, Normalize)
{

}

TEST(Float_Dual_Quat, Conjugate)
{

}

TEST(Float_Dual_Quat, Multiply)
{
   F_Quat real_1(1.0f, glm::vec3(1.1f, 2.2f, 3.3f));
   F_Quat dual_1(0.0f, glm::vec3(4.4f, 5.5f, 6.6f));
   //F_Dual_Quat dq1(real_1, dual_1);

   F_Quat real_2(1.0f, glm::vec3(-3.3f, -2.2f, -1.1f));
   F_Quat dual_2(0.0f, glm::vec3(-6.6f, -5.5f, -4.4f));
   //F_Dual_Quat dq2(real_2, dual_2);

   F_Quat AD = real_1 * dual_2;
   F_Quat BC = dual_1 * real_2;

   cout << "hello" << endl;
}
