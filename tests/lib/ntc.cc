#include <limits.h>
#include "gtest/gtest.h"
#include "../../src/lib/ntc/ntc.h"

namespace {

TEST(NTC10k, ntc10k_b3900k_temperature) {
    EXPECT_LT(ntc10k_b3900k_temperature(328997), -40);
    EXPECT_EQ(-40, ntc10k_b3900k_temperature(328996));
    EXPECT_EQ(125, ntc10k_b3900k_temperature(358));
    EXPECT_GT(ntc10k_b3900k_temperature(357), 125);
    
    EXPECT_EQ(-5, ntc10k_b3900k_temperature(41813));
    EXPECT_EQ(0, ntc10k_b3900k_temperature(32330));
    EXPECT_EQ(80, ntc10k_b3900k_temperature(1284));
    EXPECT_EQ(81, ntc10k_b3900k_temperature(1224));
    EXPECT_EQ(82, ntc10k_b3900k_temperature(1183));
    EXPECT_EQ(88, ntc10k_b3900k_temperature(1000));
    EXPECT_EQ(89, ntc10k_b3900k_temperature(961));
}

TEST(NTC10k, ntc10k_b3900k_valid_resistance) {
    EXPECT_EQ(0, ntc10k_b3900k_valid_resistance(328996));
    EXPECT_EQ(0, ntc10k_b3900k_valid_resistance(358));

    EXPECT_EQ(0, ntc10k_b3900k_valid_resistance(723));
    EXPECT_EQ(0, ntc10k_b3900k_valid_resistance(2809));

    EXPECT_EQ(-1, ntc10k_b3900k_valid_resistance(0xFFFFFFFF));
    EXPECT_EQ(1, ntc10k_b3900k_valid_resistance(0));

    EXPECT_EQ(-1, ntc10k_b3900k_valid_resistance(328997));
    EXPECT_EQ(1, ntc10k_b3900k_valid_resistance(357));
}

}
