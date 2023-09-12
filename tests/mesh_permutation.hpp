#pragma once

#include <random>
#include <chrono>
#include <algorithm>

inline triangle_mesh random_permutation(const triangle_mesh & input) {

  static constexpr int rot[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};

  static std::random_device rd;
  static std::mt19937 g(rd());

  std::vector<int> vertex_permutation(input.vertices.size());
  for (int i = 0; i < input.vertices.size(); i++) {
    vertex_permutation[i] = i;
  }
  std::shuffle(vertex_permutation.begin(), vertex_permutation.end(), g);

  std::vector<int> elem_permutation(input.vertices.size());
  for (int i = 0; i < input.triangles.size(); i++) {
    elem_permutation[i] = i;
  }
  std::shuffle(elem_permutation.begin(), elem_permutation.end(), g);

  triangle_mesh output;

  output.vertices.resize(input.vertices.size());
  for (int i = 0; i < input.vertices.size(); i++) {
    output.vertices[vertex_permutation[i]] = input.vertices[i];
  }

  output.triangles.resize(input.triangles.size());
  for (int i = 0; i < input.triangles.size(); i++) {
    auto tri = input.triangles[elem_permutation[i]];

    const int * p = rot[g() % 3];
    for (int j = 0; j < 3; j++) {
      output.triangles[i][j] = vertex_permutation[tri[p[j]]];
    }
  }

  return output;

}