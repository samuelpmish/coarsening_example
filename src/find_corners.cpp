#include "interface.hpp"
#include "type_aliases.hpp"
#include "array_functions.hpp"

#include <cmath>

std::vector<int> find_corners(const std::vector<double2> & vertices,
                              const std::vector<int2> & bdr_edges) {

  vec<int> corners;

  vec<double2> normals(vertices.size(), double2{});

  for (int2 edge : bdr_edges) {
    double2 n = unit_normal(vertices[edge[0]], vertices[edge[1]]);
    normals[edge[0]] += n;
    normals[edge[1]] += n;
  }

  for (int i = 0; i < normals.size(); i++) {
    double n = norm(normals[i]);
    if (0 < n && n < 1.8) {
      corners.push_back(i);
    }
  }

  return corners;

}