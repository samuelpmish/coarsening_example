#include "interface.hpp"
#include "array_functions.hpp"

#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>

edge_info find_edges(const std::vector< int3 > & tris) {

  std::unordered_map< int2, int, hash_fn > edges;

  int local_edge_ids[3][2] = {{0, 1}, {1, 2}, {2, 0}};

  size_t num_bdr = 0;
  size_t num_tris = tris.size();
  for (auto tri : tris) {
    for (auto e : local_edge_ids) {
      auto edge = int2{tri[e[0]], tri[e[1]]};
      auto rev = reverse(edge);
      if (edges.count(rev)) {
        edges[rev]++;
        num_bdr--;
      } else {
        edges[edge] = 1;
        num_bdr++;
      }
    }
  }

  edge_info info{vec<int2>(edges.size()), vec<int2>(num_bdr)};

  int e[2] = {0,0};
  for (auto [key, value] : edges) {

    info.edges[e[0]++] = key;

    // edges visited only once are on the boundary
    if (value == 1) info.bdr_edges[e[1]++] = key;

  }

  return info;

}