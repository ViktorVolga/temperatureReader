#include <cstdint>
#include <gtest/gtest.h>
#include <memory>

#include "../headers/RomFinder.hpp"
#include "../headers/logger.hpp"
#include "../headers/RomFinder.hpp"

TEST(isEqualSute, shuldReturnTrue)
{
    std::shared_ptr<iDS2482> ds2482 = std::make_shared<MockDS2482>();
    std::unique_ptr<RomFinder> romFinder = std::make_unique<RomFinder>(ds2482);
    uint64_t foundedRom = 0xB300000000000001;
    uint64_t currentRom = 0xB000000000000001;
    EXPECT_TRUE(romFinder->isEqual(foundedRom, currentRom, 3));

    foundedRom = 0xF300000000000001;
    EXPECT_FALSE(romFinder->isEqual(foundedRom, currentRom, 3));
}

TEST(getBit, shuldReturnOne)
{
    std::shared_ptr<iDS2482> ds2482 = std::make_shared<MockDS2482>();
    std::unique_ptr<RomFinder> romFinder = std::make_unique<RomFinder>(ds2482);
    uint64_t rom = 0xB300000000000001;
    EXPECT_EQ(romFinder->getBit(rom, 0), 1);
    EXPECT_EQ(romFinder->getBit(rom, 1), 0);
    EXPECT_EQ(romFinder->getBit(rom, 2), 1);
    EXPECT_EQ(romFinder->getBit(rom, 3), 1);
    EXPECT_EQ(romFinder->getBit(rom, 4), 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}