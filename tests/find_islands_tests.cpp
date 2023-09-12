#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include <array>
#include <algorithm>

TEST(UnitTest, find_islands_tiny_1) {

  //  3 --- 4 --- 5
  //  |   * |   * | 
  //  | *   | *   |
  //  0 --- 1 --- 2
  std::vector<int3> tris = {{0,1,4}, {0,4,3}, {1,2,5}, {1,5,4}};

  auto [edges, bdr_edges] = find_edges(tris);

  int num_vertices = 6;
  std::vector< std::vector< int > > neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : edges) {
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  auto [islands, island_markers] = find_islands(num_vertices, neighbors);

  EXPECT_EQ(islands.size(), 1);
  EXPECT_EQ(island_markers.size(), 6);
  if (island_markers.size() == 6) {
    int expected = 0;
    for (int i = 0; i < 6; i++) {
      EXPECT_EQ(island_markers[i], expected);
    }
  }

  ///////////////////////////
  ///////////////////////////
  ///////////////////////////

  std::vector< std::vector< int > > bdr_neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : bdr_edges) {
    bdr_neighbors[u].push_back(v);
    bdr_neighbors[v].push_back(u);
  }

  auto [bdr_islands, bdr_island_markers] = find_islands(num_vertices, bdr_neighbors);

  EXPECT_EQ(bdr_islands.size(), 1);
  EXPECT_EQ(bdr_island_markers.size(), 6);
  if (bdr_island_markers.size() == 6) {
    int expected = 0;
    for (int i = 0; i < 6; i++) {
      EXPECT_EQ(bdr_island_markers[i], expected);
    }
  }

}

TEST(UnitTest, find_islands_tiny_2) {

  //  4 --- 5   6 --- 7
  //  |   * |   |   * | 
  //  | *   |   | *   |
  //  0 --- 1   2 --- 3
  std::vector<int3> tris = {{0,1,5}, {0,5,4}, {2,3,7}, {2,7,6}};

  auto [edges, bdr_edges] = find_edges(tris);

  constexpr int num_vertices = 8;
  std::vector< std::vector< int > > neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : edges) {
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  auto [islands, island_markers] = find_islands(num_vertices, neighbors);

  EXPECT_EQ(islands.size(), 2);
  EXPECT_EQ(island_markers.size(), num_vertices);
  if (island_markers.size() == num_vertices) {
    int expected[num_vertices] = {0, 0, 1, 1, 0, 0, 1, 1};
    for (int i = 0; i < num_vertices; i++) {
      EXPECT_EQ(island_markers[i], expected[i]);
    }
  }

  ///////////////////////////
  ///////////////////////////
  ///////////////////////////

  std::vector< std::vector< int > > bdr_neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : bdr_edges) {
    bdr_neighbors[u].push_back(v);
    bdr_neighbors[v].push_back(u);
  }

  auto [bdr_islands, bdr_island_markers] = find_islands(num_vertices, bdr_neighbors);

  EXPECT_EQ(bdr_islands.size(), 2);
  EXPECT_EQ(bdr_island_markers.size(), num_vertices);
  if (bdr_island_markers.size() == num_vertices) {
    int expected[num_vertices] = {0, 0, 1, 1, 0, 0, 1, 1};
    for (int i = 0; i < num_vertices; i++) {
      EXPECT_EQ(bdr_island_markers[i], expected[i]);
    }
  }

}

TEST(UnitTest, find_islands_small) {

  // see data/islands_2D.jpg
  std::vector<double2> vertices = read_binary<double2>(DATA_DIR "islands_2D.vert");
  std::vector<int3> tris = read_binary<int3>(DATA_DIR "islands_2D.tri");

  auto [edges, bdr_edges] = find_edges(tris);

  int num_vertices = vertices.size();
  std::vector< std::vector< int > > neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : edges) {
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  auto [islands, island_markers] = find_islands(num_vertices, neighbors);

  EXPECT_EQ(islands.size(), 2);

  ///////////////////////////
  ///////////////////////////
  ///////////////////////////

  std::vector< std::vector< int > > bdr_neighbors(num_vertices, std::vector<int>(0));
  for (auto [u, v] : bdr_edges) {
    bdr_neighbors[u].push_back(v);
    bdr_neighbors[v].push_back(u);
  }

  auto [bdr_islands, bdr_island_markers] = find_islands(num_vertices, bdr_neighbors);

  EXPECT_EQ(bdr_islands.size(), 4);

}