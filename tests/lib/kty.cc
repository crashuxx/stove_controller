#include <limits.h>
#include "gtest/gtest.h"
#include "../../src/lib/kty/kty.h"

namespace {

TEST(Kty, kty81_210_temperature) {
  EXPECT_EQ(150, kty81_210_temperature(4280));
  EXPECT_EQ(-55, kty81_210_temperature(980));

  EXPECT_EQ(82, kty81_210_temperature(3030));
  EXPECT_EQ(80, kty81_210_temperature(2980));
  EXPECT_EQ(70, kty81_210_temperature(2785));
  EXPECT_EQ(65, kty81_210_temperature(2700));
}

TEST(Kty, kty81_210_valid_resistance) {
  EXPECT_EQ(0, kty81_210_valid_resistance(3030));
  EXPECT_EQ(0, kty81_210_valid_resistance(2700));

  EXPECT_EQ(0, kty81_210_valid_resistance(980));
  EXPECT_EQ(0, kty81_210_valid_resistance(4280));

  EXPECT_EQ(1, kty81_210_valid_resistance(0xFFFFFFFF));
  EXPECT_EQ(-1, kty81_210_valid_resistance(0));

  EXPECT_EQ(-1, kty81_210_valid_resistance(979));
  EXPECT_EQ(1, kty81_210_valid_resistance(4281));
}

}
