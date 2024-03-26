#include "gtest/gtest.h"
#include "include/buried.h"

TEST(BuriedBasicTest, Test1) { BuriedCreate("/home/chen/Code/C++/BuriedPoint/project/BuriedPoint"); }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
