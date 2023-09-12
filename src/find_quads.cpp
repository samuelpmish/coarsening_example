#include "interface.hpp"
#include "array_functions.hpp"

#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>

quad_info find_quads(const std::vector< std::array< int, 3 > > & tris) {

  std::unordered_map< int2, int, hash_fn > edges;
  std::unordered_map< int2, int2, hash_fn > auxiliary;
  std::unordered_map< int2, int2, hash_fn > tri_ids;

  int local_edge_ids[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};

  size_t num_bdr = 0;
  size_t num_tris = tris.size();
  for (int i = 0; i < tris.size(); i++) {
    auto tri = tris[i];
    for (auto e : local_edge_ids) {
      auto edge = int2{tri[e[0]], tri[e[1]]};
      auto rev = reverse(edge);
      if (edges.count(rev)) {
        edges[rev]++;
        auxiliary[rev][1] = tri[e[2]];
        tri_ids[rev][1] = i;
        num_bdr--;
      } else {
        edges[edge] = 1;
        auxiliary[edge][0] = tri[e[2]];
        tri_ids[edge][0] = i;
        num_bdr++;
      }
    }
  }

  quad_info output;


  for (auto [edge, multiplicity] : edges) {
    // quads only come from interior edges
    if (multiplicity == 2) {
      int2 aux = auxiliary[edge];
      output.quads.push_back({edge[0], aux[1], edge[1], aux[0]});
      output.tris.push_back(reverse(tri_ids[edge])); // reversed to get orientation right
    }
  }

  return output;

}