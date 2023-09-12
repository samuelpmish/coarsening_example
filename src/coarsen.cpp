#include "interface.hpp"
#include "array_functions.hpp"

#include "binary_io.hpp"

#include <algorithm>
#include <set>
#include <unordered_set>

bool write_out = false;
int random_id = -1;

triangle_mesh coarsen_by_color(const triangle_mesh & fine,
                               const vec<int2> & edges,
                               const vec<int2> & bdr_edges,
                               const vec<int> & colors) {

  size_t num_vertices = fine.vertices.size();

  std::vector< int > interior(num_vertices, 1);
  for (auto e : bdr_edges) {
    interior[e[0]] = interior[e[1]] = 0;
  }

  // boundary vertices can only merge to other boundary vertices
  // interior vertices can merge to boundary or interior vertices
  vec< vec< int > > neighbors(num_vertices, vec<int>(0));

  for (auto e : bdr_edges) {
    neighbors[e[1]].push_back(e[0]);
    neighbors[e[0]].push_back(e[1]);
  }

  for (auto e : edges) {
    if (interior[e[0]]) neighbors[e[0]].push_back(e[1]);
    if (interior[e[1]]) neighbors[e[1]].push_back(e[0]);
  }

  for (auto & n : neighbors) {
    std::sort(n.begin(), n.end());
  }

  std::vector< int > new_ids(num_vertices, -1);
  for (int v = 0; v < num_vertices; v++) {
    int id = v;

    // vertices with color > 0 are merged
    // with their nearest 0-color neighbor
    if (colors[v] != 0) {
      double2 p = fine.vertices[v];
      double min_value = 1.0e10;
      for (auto neighbor : neighbors[v]) {
        if (colors[neighbor] == 0) {
          double value = sqdist(p, fine.vertices[neighbor]);
          if (value < min_value) {
            min_value = value;
            id = neighbor;
          }
        }
      }
    }

    new_ids[v] = id;
  }

  // discard interior vertices whose neighbors 
  // all got merged into a line segment
  for (int v = 0; v < num_vertices; v++) {
    if (interior[v]) {
      std::set<int> new_neighbor_ids;
      for (auto neighbor : neighbors[v]) {
        new_neighbor_ids.insert(new_ids[neighbor]);
      }
      if (new_neighbor_ids.size() == 2 && new_neighbor_ids.count(v) == 0) {
        new_ids[v] = *new_neighbor_ids.begin();
      }
    }
  }

  write_binary(new_ids, "__new_ids.bin");

  for (int v = 0; v < num_vertices; v++) {
    int id = v;
    while (new_ids[id] != id) {
      id = new_ids[id];
    }
    new_ids[v] = id;
  }

  triangle_mesh coarse = fine;

  combine_vertices(coarse, new_ids);

  return coarse;

}

triangle_mesh coarsen_mesh(const triangle_mesh & mesh) {

  if (write_out) {
    random_id = rand() % 16;
    write_binary(mesh.triangles, std::to_string(random_id) + "_fine_tri.bin");
    write_binary(mesh.vertices, std::to_string(random_id) + "_fine_vert.bin");
  }

  // enumerate all of the edges in the mesh, 
  // and note which belong to the boundary
  auto [edges, bdr_edges] = find_edges(mesh.triangles);

  // identify boundary vertices where edges turn sharply 
  // 
  // these are important features of the mesh, so we try to ensure
  // that they are preserved in the derefinement process
  auto corners = find_corners(mesh.vertices, bdr_edges);

  // color the vertices on the boundary first, 
  // constraining the color of each corner to be 0
  auto bdr_colors = graph_coloring(mesh.vertices.size(), bdr_edges, corners);

  // get the indices of boundary vertices with color == 0
  std::vector<int> prescribed_colors;
  for (int i = 0; i < bdr_colors.size(); i++) {
    if (bdr_colors[i] == 0) prescribed_colors.push_back(i);
  }

  // color all of the vertices, but keep the color == 0 boundary vertices
  auto colors = graph_coloring(mesh.vertices.size(), edges, prescribed_colors);

  auto coarse = coarsen_by_color(mesh, edges, bdr_edges, colors);

  if (write_out) {
    write_binary(coarse.triangles, std::to_string(random_id) + "_coarse_tri.bin");
    write_binary(coarse.vertices, std::to_string(random_id) + "_coarse_vert.bin");
  }

  return coarse;

}
