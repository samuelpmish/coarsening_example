#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include <array>
#include <algorithm>

TEST(UnitTest, find_quads_tiny) {

  //  3 --- 4 --- 5
  //  |   * |   * | 
  //  | *   | *   |
  //  0 --- 1 --- 2
  std::vector<int3> tris = {{0,1,4}, {0,4,3}, {1,2,5}, {1,5,4}};

  auto [quads, parent_tris] = find_quads(tris);

  EXPECT_EQ(quads.size(), 3);
  if (quads.size() == 3) {
    std::array<int, 4> quads_expected[3] = {
      {5, 4, 1, 2}, {4, 3, 0, 1}, {1, 5, 4, 0}
    };

    std::array<int, 2> tris_expected[3] = {
      {3, 2}, {1, 0}, {3, 0}
    };
    for (int i = 0; i < 3; i++) {
      EXPECT_EQ(quads[i], quads_expected[i]);
      EXPECT_EQ(parent_tris[i], tris_expected[i]);
    }
  }



}
