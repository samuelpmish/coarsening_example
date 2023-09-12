#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include <array>

TEST(UnitTest, corner_detection_small) {

  std::vector<double2> vertices = {{0,0}, {1,0}, {2,0}, {0,1}, {1,1}, {2,1}};

  std::vector<int2> bdr_edges = {{0,1}, {1,2}, {2,5}, {5,4}, {4, 3}, {3, 0}};

  auto corners = find_corners(vertices, bdr_edges);

  EXPECT_EQ(corners.size(), 4);

  if (corners.size() == 4) {
    EXPECT_EQ(corners[0], 0);
    EXPECT_EQ(corners[1], 2);
    EXPECT_EQ(corners[2], 3);
    EXPECT_EQ(corners[3], 5);
  }

}

TEST(UnitTest, corner_detection_medium) {

  auto bdr_edges = read_binary<int2>(DATA_DIR "bdrEdges.bin");
  auto vertices = read_binary<double2>(DATA_DIR "vertices.bin");

  auto corners = find_corners(vertices, bdr_edges);

  EXPECT_EQ(corners.size(), 8);

}
