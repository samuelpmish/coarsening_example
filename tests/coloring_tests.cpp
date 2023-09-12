#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include <array>

TEST(UnitTest, graph_coloring_tiny) {

  std::vector<double2> vertices = {{0,0}, {1,0}, {2,0}, {0,1}, {1,1}, {2,1}};
  std::vector<int2> edges = {{0,1}, {1,2}, {0,3}, {1, 4}, {2, 5}, {3, 4}, {4, 5}};
  std::vector<int> prescribed = {0};

  std::vector<int> colors = graph_coloring(vertices.size(), edges, prescribed);

  std::cout << colors[3] << " " << colors[4] << " " << colors[5] << std::endl;
  std::cout << colors[0] << " " << colors[1] << " " << colors[2] << std::endl;

}

TEST(UnitTest, graph_coloring_2D) {

  auto edges = read_binary<int2>(DATA_DIR "edges.bin");
  auto corners = read_binary<int>(DATA_DIR "corners.bin");
  auto vertices = read_binary<double2>(DATA_DIR "vertices.bin");

  std::cout << "num vertices: " << vertices.size() << std::endl;
  std::cout << "num edges: " << edges.size() << std::endl;
  std::cout << "num corners: " << corners.size() << std::endl;

  std::vector<int> colors = graph_coloring(vertices.size(), edges, corners);
  for (int i = 0; i < colors.size(); i++) {
    std::cout << i << " " << colors[i] << std::endl;
  }

}

TEST(UnitTest, graph_coloring_2D_wrench) {

  auto edges = read_binary<int2>(DATA_DIR "wrench_edges.bin");
  auto corners = read_binary<int>(DATA_DIR "wrench_corners.bin");
  auto vertices = read_binary<double2>(DATA_DIR "wrench_vertices.bin");

  std::cout << "num vertices: " << vertices.size() << std::endl;
  std::cout << "num edges: " << edges.size() << std::endl;
  std::cout << "num corners: " << corners.size() << std::endl;

  std::vector<int> colors = graph_coloring(vertices.size(), edges, corners);
  for (int i = 0; i < corners.size(); i++) {
    EXPECT_EQ(colors[corners[i]], 0);
  }

}

TEST(UnitTest, boundary_graph_coloring_2D_wrench) {

  auto bdr_edges = read_binary<int2>(DATA_DIR "wrench_bdr_edges.bin");
  auto corners = read_binary<int>(DATA_DIR "wrench_corners.bin");
  auto vertices = read_binary<double2>(DATA_DIR "wrench_vertices.bin");

  std::cout << "num vertices: " << vertices.size() << std::endl;
  std::cout << "num bdr_edges: " << bdr_edges.size() << std::endl;
  std::cout << "num corners: " << corners.size() << std::endl;

  std::vector<int> colors = graph_coloring(vertices.size(), bdr_edges, corners);
  for (int i = 0; i < corners.size(); i++) {
    EXPECT_EQ(colors[corners[i]], 0);
  }

}
