#include <gtest\gtest.h>

int main(int argc, char **argv)
{
   // initialize the google test suit from the top level namespace
   // Note: To run these tests, this project must be the startup project.
   ::testing::InitGoogleTest(&argc, argv);
   RUN_ALL_TESTS();
}