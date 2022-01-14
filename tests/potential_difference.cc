#include <limits.h>
#include "gtest/gtest.h"


namespace {

unsigned int potential_differencef(unsigned int r_value, float resolution, float r1) {
    return r_value/(resolution-r_value)*r1;
}

unsigned int potential_difference(unsigned int r_value, unsigned int resolution, unsigned int r1) {
    return potential_differencef(r_value, (float)resolution, (float)r1);
}

TEST(misc, potential_difference) {
    EXPECT_EQ(1000, potential_difference(4095/3, 4095, 2000));
    EXPECT_NEAR(1116, potential_difference(1466, 4095, 2000), 3);
    EXPECT_NEAR(1347, potential_difference(1647, 4095, 2000), 3);
}

}
