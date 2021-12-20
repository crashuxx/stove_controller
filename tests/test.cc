#include <limits.h>
#include "gtest/gtest.h"
#include "../src/lib/estyma/estyma.h"

namespace {

TEST(Estyme, estyma_ct2_temperature) {
  EXPECT_EQ(80, estyma_ct2_temperature(1211));
  EXPECT_EQ(70, estyma_ct2_temperature(1695));
}

TEST(Estyme, estyma_ct2_valid_resistance) {
  EXPECT_EQ(0, estyma_ct2_valid_resistance(1211));
  EXPECT_EQ(0, estyma_ct2_valid_resistance(1695));

  EXPECT_EQ(0, estyma_ct2_valid_resistance(329927));
  EXPECT_EQ(0, estyma_ct2_valid_resistance(153));

  EXPECT_EQ(-1, estyma_ct2_valid_resistance(0xFFFFFFFF));
  EXPECT_EQ(1, estyma_ct2_valid_resistance(0));

  EXPECT_EQ(-1, estyma_ct2_valid_resistance(329928));
  EXPECT_EQ(1, estyma_ct2_valid_resistance(152));
}

}