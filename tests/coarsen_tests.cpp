#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include "print.hpp"

#include <array>
#include <algorithm>

TEST(UnitTest, coarsen_by_color_tiny) {

  //  3 --- 4 --- 5
  //  |   * |   * | 
  //  | *   | *   |
  //  0 --- 1 --- 2
  std::vector<double2> verts = {{0,0}, {1,0}, {2,0}, {0,1}, {1,1}, {2,1}};
  std::vector<int3> tris = {{0,1,4}, {0,4,3}, {1,2,5}, {1,5,4}};
  std::vector<int> colors = {0, 1, 0, 0, 2, 0};

  auto [edges, bdr_edges] = find_edges(tris);

  auto coarse = coarsen_by_color(triangle_mesh{verts, tris}, edges, bdr_edges, colors);

  EXPECT_EQ(coarse.vertices.size(), 4);
  if (coarse.vertices.size() == 4) {
    double2 expected[4] = {{0,0}, {2,0}, {0,1}, {2,1}};
    for (int i = 0; i < 4; i++) {
      EXPECT_EQ(coarse.vertices[i], expected[i]);
    }
  }

  EXPECT_EQ(coarse.triangles.size(), 2);
  if (coarse.triangles.size() == 2) {
    int3 expected[2] = {{0,1,3}, {0,3,2}};
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(coarse.triangles[i], expected[i]);
    }
  }

  for (auto v : coarse.vertices) {
    std::cout << v[0] << " " << v[1] << std::endl;
  }

  for (auto tri : coarse.triangles) {
    std::cout << tri[0] << " " << tri[1] << " " << tri[2] << std::endl;
  }

}

TEST(UnitTest, coarsen_by_color_medium) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "notched_plate_with_hole_vert.bin"),
    read_binary<int3>(DATA_DIR "notched_plate_with_hole_tri.bin")
  };

  auto colors = read_binary<int>(DATA_DIR "notched_plate_with_hole_color.bin");

  auto [edges, bdr_edges] = find_edges(mesh.triangles);
  auto coarse = coarsen_by_color(mesh, edges, bdr_edges, colors);
  //write_binary(coarse.triangles, "coarse_tri.bin");
  //write_binary(coarse.vertices, "coarse_vert.bin");

  //for (auto v : coarse.vertices) {
  //  std::cout << v << std::endl;
  //}

  //for (auto tri : coarse.triangles) {
  //  std::cout << tri << std::endl;
  //}

}

TEST(UnitTest, coarsen_medium) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "notched_plate_with_hole_vert.bin"),
    read_binary<int3>(DATA_DIR "notched_plate_with_hole_tri.bin")
  };

  for (int i = 0; i < 4; i++) {
    mesh = coarsen_mesh(mesh);
    write_binary(mesh.triangles, std::to_string(i) + "_coarse_tri.bin");
    write_binary(mesh.vertices, std::to_string(i) + "_coarse_vert.bin");
    EXPECT_TRUE(check_connectivity(mesh));
  }

  //for (auto v : coarse.vertices) {
  //  std::cout << v << std::endl;
  //}
  //for (auto tri : coarse.triangles) {
  //  std::cout << tri << std::endl;
  //}

}

TEST(UnitTest, error_1) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "error_1_vert.bin"),
    read_binary<int3>(DATA_DIR "error_1_tri.bin")
  };

  for (int i = 0; i < 4; i++) {
    mesh = coarsen_mesh(mesh);
    improve_connectivity(mesh);
    //write_binary(mesh.triangles, std::to_string(i) + "_coarse_tri.bin");
    //write_binary(mesh.vertices, std::to_string(i) + "_coarse_vert.bin");
    EXPECT_TRUE(check_connectivity(mesh));
  }

}
