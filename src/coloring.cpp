#include "interface.hpp"
#include "type_aliases.hpp"

#include <set>
#include <vector>
#include <iostream>
#include <unordered_set>

template < typename T >
using vec = std::vector<T>;

vec<int> graph_coloring(const size_t num_vertices,
                        const vec<int2> & edges,
                        const vec<int> & prescribed_colors) {

  constexpr int unvisited = -2;
  constexpr int queued = -1;

  vec< vec< int > > neighbors(num_vertices, vec<int>(0));
  for (int i = 0; i < edges.size(); i++) {
    auto [u, v] = edges[i];
    neighbors[u].push_back(v);
    neighbors[v].push_back(u);
  }

  vec<int> colors(num_vertices, unvisited);

  // append all unvisited nodes connected node `i` to `list`
  auto append = [&](vec<int> & list, int i) {
    for (auto v : neighbors[i]) {
      if (colors[v] == unvisited) {
        colors[v] = queued;
        list.push_back(v);
      } 
    }
  };

  // enforce the prescribed constraints
  vec< int > active;
  if (prescribed_colors.size() > 0) {
    for (auto v : prescribed_colors) {
      colors[v] = 0;
      append(active, v);
    }
  }
  
  // we want to make sure that all "islands" have at least one
  // active point, otherwise some vertices won't get a color
  auto [islands, island_markers] = find_islands(num_vertices, neighbors);
  
  vec<int> island_seeded(islands.size(), 0);
  for (auto v : active) {
    int id = island_markers[v];
    island_seeded[id] = 1;
  }

  // make sure each island has at least one active vertex
  for (int i = 0; i < islands.size(); i++) {
    if (island_seeded[i] == 0) {
      append(active, islands[i][0]);
    }
  }

  auto find_color = [&](int v){

    // note: this only works with up to 64 colors, but 
    //       that is more than enough for finite element meshes
    uint64_t taken = 0;

    // note which colors have been taken so far
    for (auto u : neighbors[v]) {
      if (colors[u] >= 0) {
        taken |= (uint64_t(1) << colors[u]);
      }
    }

    // and take the smallest available one
    for (int i = 0; i < 64; i++) {
      if ((taken & (uint64_t(1) << i)) == 0) {
        return i;
      }
    }

    return 1000;

  };

  while (active.size() > 0) {
    vec<int> next;
    for (auto v : active) {
      if (colors[v] < 0) {
        colors[v] = find_color(v);
        append(next, v);
      }
    }
    active.swap(next);
  }

  return colors;

}

