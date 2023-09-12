#include "interface.hpp"
#include "type_aliases.hpp"

bool check_connectivity(const triangle_mesh & mesh) {

  auto [edges, bdr_edges] = find_edges(mesh.triangles);

  vec<int> interior(mesh.vertices.size(), 1);
  for (auto e : bdr_edges) {
    interior[e[0]] = interior[e[1]] = 0;
  }

  vec<int> element_count(mesh.vertices.size());

  for (auto tri : mesh.triangles) {
    element_count[tri[0]]++;
    element_count[tri[1]]++;
    element_count[tri[2]]++;
  }

  for (std::size_t i = 0; i < mesh.vertices.size(); i++) {
    if (interior[i] == 1 && element_count[i] == 1) return false;
  }

  return true;

}