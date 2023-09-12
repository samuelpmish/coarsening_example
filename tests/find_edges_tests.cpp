#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include <array>
#include <algorithm>

TEST(UnitTest, find_edges_tiny) {

  //  3 --- 4 --- 5
  //  |   * |   * | 
  //  | *   | *   |
  //  0 --- 1 --- 2
  std::vector<int3> tris = {{0,1,4}, {0,4,3}, {1,2,5}, {1,5,4}};

  auto [edges, bdr_edges] = find_edges(tris);

  std::sort(edges.begin(), edges.end());

  EXPECT_EQ(edges.size(), 9);
  if (edges.size() == 9) {
    int2 expected[9] = {
      {0, 1}, {1, 2}, {1, 4}, 
      {2, 5}, {3, 0}, {4, 0}, 
      {4, 3}, {5, 1}, {5, 4}
    };
    for (int i = 0; i < 9; i++) {
      EXPECT_EQ(edges[i], expected[i]);
    }
  }

  std::sort(bdr_edges.begin(), bdr_edges.end());

  EXPECT_EQ(bdr_edges.size(), 6);
  if (bdr_edges.size() == 6) {
    int2 expected[6] = {
      {0, 1}, {1, 2}, {2, 5},
      {3, 0}, {4, 3}, {5, 4}
    };
    for (int i = 0; i < 6; i++) {
      EXPECT_EQ(bdr_edges[i], expected[i]);
    }
  }

}
