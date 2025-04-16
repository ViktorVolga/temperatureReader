#include <gtest/gtest.h>

#include "../headers/RomFinder.hpp"
#include "../headers/logger.hpp"
#include "../headers/RomFinder.hpp"

TEST(getNextBitSut, shuldReturn1){

    EXPECT_EQ(0x01 << 0, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}