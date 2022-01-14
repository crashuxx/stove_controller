#include <limits.h>
#include "gtest/gtest.h"
#include "../../src/lib/pt/pt.h"

namespace {

TEST(PT, pt1000_temperature) {
    EXPECT_EQ(-70, pt1000_temperature(723));
    EXPECT_EQ(500, pt1000_temperature(2809));
    
    EXPECT_EQ(-5, pt1000_temperature(979));
    EXPECT_EQ(0, pt1000_temperature(1000));
    EXPECT_EQ(80, pt1000_temperature(1309));
    EXPECT_EQ(81, pt1000_temperature(1313));
    EXPECT_EQ(82, pt1000_temperature(1317));
    EXPECT_EQ(88, pt1000_temperature(1340));
    EXPECT_EQ(89, pt1000_temperature(1344));
}

TEST(PT, pt1000_valid_resistance) {
    EXPECT_EQ(0, pt1000_valid_resistance(1030));
    EXPECT_EQ(0, pt1000_valid_resistance(2700));

    EXPECT_EQ(0, pt1000_valid_resistance(723));
    EXPECT_EQ(0, pt1000_valid_resistance(2809));

    EXPECT_EQ(1, pt1000_valid_resistance(0xFFFFFFFF));
    EXPECT_EQ(-1, pt1000_valid_resistance(0));

    EXPECT_EQ(-1, pt1000_valid_resistance(722));
    EXPECT_EQ(1, pt1000_valid_resistance(2810));
}

}
