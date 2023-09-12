#pragma once

#include <array>
#include <vector>

struct quad_info {
  std::vector< std::array<int, 4 > > quads;
  std::vector< std::array<int, 2 > > tris;
};

struct edge_info {
  std::vector< std::array<int, 2 > > edges;
  std::vector< std::array<int, 2 > > bdr_edges;
};

struct island_info {
  std::vector< std::vector<int> > islands;
  std::vector<int> id;
};

struct triangle_mesh {
  std::vector< std::array< double, 2 > > vertices;
  std::vector< std::array< int, 3 > > triangles;
};

std::vector<int> find_corners(const std::vector< std::array< double, 2 > > & vertices,
                              const std::vector< std::array< int, 2 > > & bdr_edges);

edge_info find_edges(const std::vector< std::array< int, 3 > > & tris);

quad_info find_quads(const std::vector< std::array< int, 3 > > & tris);

island_info find_islands(int num_vertices, std::vector< std::vector< int > > & neighbors);

std::vector<int> graph_coloring(const std::size_t num_vertices,
                                const std::vector< std::array<int, 2> > & edges,
                                const std::vector<int> & prescribed_colors);

void dvr(triangle_mesh & mesh, 
         const std::vector<int> & on_interior, 
         float alpha = 10.0f, 
         float step = 0.05f, 
         int ndvr = 5);

bool check_connectivity(const triangle_mesh & mesh);

void combine_vertices(triangle_mesh & mesh, const std::vector<int> & new_ids);

void improve_connectivity(triangle_mesh & mesh);

void quad_flips(triangle_mesh & mesh);

// note: this fn assumes every vertex with color > 1
//       shares an edge with a vertex with color == 1
//       
//       this property is guaranteed by graph_coloring()
triangle_mesh coarsen_by_color(const triangle_mesh & fine,
                               const std::vector< std::array<int,2> > & edges,
                               const std::vector< std::array<int,2> > & bdr_edges,
                               const std::vector<int> & colors);

triangle_mesh coarsen_mesh(const triangle_mesh & mesh);