#include "interface.hpp"

#include <queue>

#include <iostream>

template < typename T >
using vec = std::vector<T>;

island_info find_islands(int num_vertices, std::vector< std::vector< int > > & neighbors) {

  constexpr int unvisited = -1;

  island_info info {{}, std::vector<int>(num_vertices, unvisited)};

  auto mark_all_connected = [&](int v0, int id) {
    std::vector<int> island;
    std::queue<int> q;
    q.push(v0);
    info.id[v0] = id;
    island.push_back(v0);

    while (q.size() > 0) {
      int v = q.front(); q.pop();
      for (auto neighbor : neighbors[v]) {
        if (info.id[neighbor] == unvisited) {
          q.push(neighbor);
          info.id[neighbor] = id;
          island.push_back(neighbor);
        }
      }
    }
    return island;
  };

  for (int v = 0; v < neighbors.size(); v++) {
    if (neighbors[v].size() > 0) {
      if (info.id[neighbors[v][0]] == unvisited) {
        info.islands.push_back(mark_all_connected(v, info.islands.size()));
      }
    }
  }

  return info;

}
