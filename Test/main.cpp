#include "gtest\gtest.h"

/* Unit Test headers */
#include "T_BehaviorTree.h"

/* END Unit Test headers */

#include <iostream>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Completed testing." << std::endl;
    return result;
}