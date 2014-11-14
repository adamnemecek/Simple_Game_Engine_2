// for the Google Test frame
#include <gtest\gtest.h>

// for the class under test
#include <Engine_2\Math\F_Dual_Number.h>
using Math::F_Dual_Number;


TEST(Float_Dual_Number, Default_Constructor)
{
   F_Dual_Number dn;

   EXPECT_FLOAT_EQ(0.0f, dn.m_real);
   EXPECT_FLOAT_EQ(0.0f, dn.m_dual);
}

TEST(Float_Dual_Number, Explicit_Constructor)
{
   F_Dual_Number dn(1.1f, 2.2f);

   EXPECT_FLOAT_EQ(1.1f, dn.m_real);
   EXPECT_FLOAT_EQ(2.2f, dn.m_dual);
}
