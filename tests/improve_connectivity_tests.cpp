#include <gtest/gtest.h>

#include "binary_io.hpp"
#include "interface.hpp"
#include "type_aliases.hpp"

#include "mesh_permutation.hpp"

#include "print.hpp"

#include <array>
#include <algorithm>

TEST(UnitTest, improvement3) {

  //        2
  //      * | *
  //     *  |  *
  //    *   |   *  
  //   *    0    *  
  //  *  *     *  * 
  // 3 ----------- 1
  triangle_mesh mesh{
    {{0,0}, {1,-0.5}, {0,1}, {-1,-0.5}}, // vertices
    {{0,1,2}, {0,2,3}, {0,3,1}}          // triangles
  };

  improve_connectivity(mesh);

  EXPECT_EQ(mesh.vertices.size(), 3);
  if (mesh.vertices.size() == 3) {
    double2 expected[3] = {{1,-0.5}, {0,1}, {-1,-0.5}};
    for (int i = 0; i < 3; i++) {
      EXPECT_EQ(mesh.vertices[i], expected[i]);
    }
  }

  EXPECT_EQ(mesh.triangles.size(), 1);

}

TEST(UnitTest, improvement4) {

  //        1
  //      * | *
  //    *   |   *
  //  2 --- 4 --- 0
  //    *   |   *  
  //      * | *    
  //        3      
  triangle_mesh mesh{
    {{1,0}, {0,1}, {-1,0}, {0,-1}, {0,0}}, // vertices
    {{0,1,4}, {1,2,4}, {2,3,4}, {3,0,4}}   // triangles
  };

  improve_connectivity(mesh);

  EXPECT_EQ(mesh.vertices.size(), 4);
  if (mesh.vertices.size() == 4) {
    double2 expected[4] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    for (int i = 0; i < 4; i++) {
      EXPECT_EQ(mesh.vertices[i], expected[i]);
    }
  }

  EXPECT_EQ(mesh.triangles.size(), 2);


}

TEST(UnitTest, improvement6) {

  //          4
  //       * * * *
  //    *   *   *   *  
  // 0 --- 1 --- 2 --- 3
  //    *   *   *   *  
  //       * * * *
  //          5
  triangle_mesh mesh{
    {{-3,0}, {-1,0}, {1,0}, {3,0}, {0,2}, {0,-2}},
    {{0,1,4}, {1,2,4}, {2,3,4}, {1,0,5}, {2,1,5}, {3,2,5}}
  };

  for (int i = 0; i < 100; i++) {
    auto permuted_mesh = random_permutation(mesh);
    auto improved_mesh = permuted_mesh;
    improve_connectivity(improved_mesh);

    auto [edges, bdr_edges] = find_edges(improved_mesh.triangles);
    EXPECT_EQ(bdr_edges.size(), 4);
    EXPECT_EQ(improved_mesh.vertices.size(), 4);
    EXPECT_EQ(improved_mesh.triangles.size(), 2);

    if (bdr_edges.size() != 4 || improved_mesh.vertices.size() != 4 || improved_mesh.triangles.size() != 2) {
      write_binary(permuted_mesh.triangles, std::to_string(i) + "_mesh_before_tri.bin");
      write_binary(permuted_mesh.vertices, std::to_string(i) + "_mesh_before_vert.bin");

      write_binary(improved_mesh.triangles, std::to_string(i) + "_mesh_after_tri.bin");
      write_binary(improved_mesh.vertices, std::to_string(i) + "_mesh_after_vert.bin");
    }
  }

}

TEST(UnitTest, improvement_error_1) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "improvement_error_1_vert.bin"),
    read_binary<int3>(DATA_DIR "improvement_error_1_tri.bin")
  };

  improve_connectivity(mesh);

  auto [edges, bdr_edges] = find_edges(mesh.triangles);
  EXPECT_EQ(bdr_edges.size(), 4);
  EXPECT_EQ(mesh.vertices.size(), 4);
  EXPECT_EQ(mesh.triangles.size(), 2);

  //write_binary(mesh.vertices, DATA_DIR "improvement_output_1_vert.bin");
  //write_binary(mesh.triangles, DATA_DIR "improvement_output_1_tri.bin");

}

TEST(UnitTest, improvement_error_2) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "improvement_error_2_vert.bin"),
    read_binary<int3>(DATA_DIR "improvement_error_2_tri.bin")
  };

  improve_connectivity(mesh);

  auto [edges, bdr_edges] = find_edges(mesh.triangles);
  EXPECT_EQ(bdr_edges.size(), 4);
  EXPECT_EQ(mesh.vertices.size(), 4);
  EXPECT_EQ(mesh.triangles.size(), 2);

  //write_binary(mesh.vertices, DATA_DIR "improvement_output_2_vert.bin");
  //write_binary(mesh.triangles, DATA_DIR "improvement_output_2_tri.bin");

}

TEST(UnitTest, improvement_error_3) {

  triangle_mesh mesh{
    read_binary<double2>(DATA_DIR "improvement_error_3_vert.bin"),
    read_binary<int3>(DATA_DIR "improvement_error_3_tri.bin")
  };

  improve_connectivity(mesh);

  auto [edges, bdr_edges] = find_edges(mesh.triangles);
  EXPECT_EQ(bdr_edges.size(), 4);
  EXPECT_EQ(mesh.vertices.size(), 4);
  EXPECT_EQ(mesh.triangles.size(), 2);

  //write_binary(mesh.vertices, DATA_DIR "improvement_output_3_vert.bin");
  //write_binary(mesh.triangles, DATA_DIR "improvement_output_3_tri.bin");

}