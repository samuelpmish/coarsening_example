#include "interface.hpp"

#include "timer.hpp"
#include "array_functions.hpp"
#include "threadpool.hpp"

#include <iostream>
#include <set>
#include <unordered_set>

bool print_timings = true;

float quality(const triangle & x) {
  double2 L01 = x[1] - x[0];
  double2 L12 = x[2] - x[1];
  double2 L20 = x[0] - x[2];

  float top = det(L20, L01);
  float bot = dot(L01, L01) + dot(L12, L12) + dot(L20, L20);

  return top / bot;
}

std::tuple<float, triangle> quality_and_gradient(const triangle & x) {

  double2 L01 = x[1] - x[0];
  double2 L12 = x[2] - x[1];
  double2 L20 = x[0] - x[2];

  float top = det(L20, L01);
  auto dtop_dx = triangle{{{ x[1][1] - x[2][1], -x[1][0] + x[2][0]},
                           {-x[0][1] + x[2][1],  x[0][0] - x[2][0]},
                           { x[0][1] - x[1][1], -x[0][0] + x[1][0]}}};

  float bot = dot(L01, L01) + dot(L12, L12) + dot(L20, L20);
  auto dbot_dx =
      triangle{{{4 * x[0][0] - 2 * x[1][0] - 2 * x[2][0],
                 4 * x[0][1] - 2 * x[1][1] - 2 * x[2][1]},
                {-2 * x[0][0] + 4 * x[1][0] - 2 * x[2][0],
                 -2 * x[0][1] + 4 * x[1][1] - 2 * x[2][1]},
                {-2 * x[0][0] - 2 * x[1][0] + 4 * x[2][0],
                 -2 * x[0][1] - 2 * x[1][1] + 4 * x[2][1]}}};

  constexpr float scale = 3.4641016151377543864;  // 2 * sqrt(3)

  return {
    (top / bot) * scale,
    (dtop_dx / bot - dbot_dx * (top / (bot * bot))) * scale
  };
}

void dvr(triangle_mesh & mesh, const std::vector<int> & interior, float alpha, float step, int ndvr) {

  int num_threads = 1; // TODO
  int num_triangles = mesh.triangles.size();

  timer stopwatch;
  stopwatch.start();

  if (ndvr > 0) {

    constexpr int nmutex = 128;
    std::mutex mtx[nmutex];

    threadpool pool(num_threads);
    std::vector< std::thread > threads;

    for (int k = 0; k < ndvr; k++) {

      std::vector< float > scale(mesh.vertices.size(), 0.0);
      std::vector< double2 > grad(mesh.vertices.size(), double2{});

      auto calculate_scale_and_grad = [&](uint32_t i) {
        int3 elem_ids = mesh.triangles[i];
        triangle tri = {
          mesh.vertices[elem_ids[0]],
          mesh.vertices[elem_ids[1]],
          mesh.vertices[elem_ids[2]]
        };

        auto [Q, dQdX] = quality_and_gradient(tri);

        float expQ = expf(-alpha * Q);

        dQdX = dQdX * expQ;
        for (int j = 0; j < 3; j++) {
          double2 g = dQdX[j];

          int which = elem_ids[j] % nmutex;
          mtx[which].lock();
          grad[elem_ids[j]] += g;
          scale[elem_ids[j]] += expQ;
          mtx[which].unlock();
        }
      };

      if (num_threads == 1) {
        for (int i = 0; i < num_triangles; i++) {
          calculate_scale_and_grad(i);
        }
      } else {
        pool.parallel_for(mesh.triangles.size(), calculate_scale_and_grad);
      }

      for (uint64_t i = 0; i < grad.size(); i++) {
        mesh.vertices[i] += grad[i] * (interior[i] * step / scale[i]);
      }

    }

  }

  stopwatch.stop();

  if (print_timings) {
    std::cout << "dvr iterations: " << stopwatch.elapsed() * 1000 << "ms" << std::endl;
  } 

}

void combine_vertices(triangle_mesh & mesh, const std::vector<int> & new_ids) {

  std::vector<int> keep(new_ids.size(), -1);
  std::vector< double2 > updated_vertices;
  for (std::size_t i = 0; i < mesh.vertices.size(); i++) {
    keep[new_ids[i]] = 1;
  }

  std::vector<int> sum(new_ids.size());
  for (std::size_t i = 0; i < mesh.vertices.size(); i++) {
    sum[i] = updated_vertices.size();
    if (keep[i] >= 0) {
      updated_vertices.push_back(mesh.vertices[i]);
    }
  }

  std::vector<int> compact_ids(new_ids.size());
  for (std::size_t i = 0; i < mesh.vertices.size(); i++) {
    compact_ids[i] = sum[new_ids[i]];
  }

  // example:
  // 0 1 2 3 4 5 6 7 ids
  // 0 0 7 3 3 2 2 3 new_ids
  // 
  // 1 0 1 1 0 0 0 1 keep
  // 0 1 1 2 3 3 3 3 sum
  // 0 0 3 2 2 1 1 1 compact_ids

  std::unordered_set< int3, hash_fn > uniq_tris;

  // getting rid of vertices screws up the element connectivity arrays,
  // so we need to figure out the updated indices for each element
  std::vector< int3 > updated_triangles;
  for (int3 tri : mesh.triangles) {
    for (auto & v : tri) {
      v = compact_ids[v];
    }

    // don't add degenerate or duplicated elements to the mesh
    bool is_degenerate = (tri[0] == tri[1] || 
                          tri[1] == tri[2] || 
                          tri[0] == tri[2]);
    if (is_degenerate || uniq_tris.count(sort(tri))) {
      continue;
    } else {
      uniq_tris.insert(sort(tri));
      updated_triangles.push_back(tri);
    }
  }

  mesh.vertices = updated_vertices;
  mesh.triangles = updated_triangles;

}

void quad_flips(triangle_mesh & mesh) {

  auto [quads, tris] = find_quads(mesh.triangles);

  auto & v = mesh.vertices;

  for (int i = 0; i < quads.size(); i++) {
    auto quad = quads[i]; 
    double2 p[4] = {v[quad[0]], v[quad[1]], v[quad[2]], v[quad[3]]};
    auto [tri0, tri1] = tris[i]; 

    double q1 = std::min(quality({p[0], p[1], p[2]}), quality({p[0], p[2], p[3]}));
    double q2 = std::min(quality({p[0], p[1], p[3]}), quality({p[1], p[2], p[3]}));

    // if the flipped orientation has better quality
    if (q2 > q1) {
      // then update the triangle connectivity to use that orientation
      mesh.triangles[tri0] = {quad[0], quad[1], quad[3]};
      mesh.triangles[tri1] = {quad[1], quad[2], quad[3]};
    }
  }

}

void improve_connectivity(triangle_mesh & mesh) {

  size_t num_vertices = mesh.vertices.size();

  auto [edges, bdr_edges] = find_edges(mesh.triangles);

  vec<int> interior(num_vertices, 1);

  for (int2 e : bdr_edges) {
    interior[e[0]] = interior[e[1]] = 0;
  }

  vec< vec< int > > neighbors(num_vertices, vec<int>(0));
  for (int2 e : edges) {
    neighbors[e[0]].push_back(e[1]);
    neighbors[e[1]].push_back(e[0]);
  }

  // merge 3- and 4-valence interior vertices to one of their neighbors
  int num_eliminated = 0;
  std::vector< int > new_ids(num_vertices);
  for (int i = 0; i < mesh.vertices.size(); i++) {
    new_ids[i] = i;
  }

  for (int i = 0; i < mesh.vertices.size(); i++) {
    if (interior[i] && (neighbors[i].size() <= 4)) {
      for (auto neighbor : neighbors[i]) {
        // don't merge with a neighbor that has 
        // also merged to another vertex
        if (new_ids[neighbor] == neighbor) {
          new_ids[i] = neighbor;
        }
      }
      num_eliminated++;
    }
  }

  for (int i = 0; i < mesh.vertices.size(); i++) {
    int id = new_ids[i];
    while (new_ids[id] != id) {
      id = new_ids[id];
    }
    new_ids[i] = id;
  }

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

  combine_vertices(mesh, new_ids);

  if (num_eliminated > 0) {
    improve_connectivity(mesh);
  }

}