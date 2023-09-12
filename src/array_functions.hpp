#pragma once

#include <cmath>

#include "type_aliases.hpp"

struct hash_fn {
  template < std::size_t n >
  std::size_t operator()(const std::array<int, n>& a) const {
    std::size_t h = 0;
    for (auto e : a) {
      h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2); 
    }
    return h;
  }   
};

inline int2 reverse(int2 x) { return {x[1], x[0]}; }

inline int3 sort(int3 x) { 
  int3 out = x;
  if (out[1] > out[0]) std::swap(out[1], out[0]);
  if (out[2] > out[0]) std::swap(out[2], out[0]);
  if (out[2] > out[1]) std::swap(out[2], out[1]);
  return out;
}

inline double norm(double2 v) {
  return std::sqrt(v[0] * v[0] + v[1] * v[1]);
}

// effectively rotate_90_degrees_CW(normalize(b-a))
inline double2 unit_normal(double2 a, double2 b) {
  double2 d = {b[0] - a[0], b[1] - a[1]};
  double norm_d = norm(d);
  return {d[1] / norm_d, -d[0] / norm_d};
}

inline void operator+=(double2 & a, double2 b) {
  a[0] += b[0];
  a[1] += b[1];
}

inline double sqdist(double2 u, double2 v) {
  double dx = v[0] - u[0];
  double dy = v[1] - u[1];
  return dx * dx + dy * dy;
}

inline double dot(double2 x, double2 y) {
  return x[0] * y[0] + x[1] * y[1];
}

inline double det(double2 x, double2 y) {
  return x[0] * y[1] - x[1] * y[0];
}

inline double2 operator-(double2 x, double2 y) {
  return {x[0] - y[0], x[1] - y[1]};
}

inline double2 operator*(double2 x, double scale) {
  return {x[0] * scale, x[1] * scale};
}

inline triangle operator*(triangle t, double scale) {
  return triangle{{{t[0][0] * scale, t[0][1] * scale},
                   {t[1][0] * scale, t[1][1] * scale},
                   {t[2][0] * scale, t[2][1] * scale}}};
}

inline triangle operator/(triangle t, double scale) {
  return triangle{{{t[0][0] / scale, t[0][1] / scale},
                   {t[1][0] / scale, t[1][1] / scale},
                   {t[2][0] / scale, t[2][1] / scale}}};
}

inline triangle operator-(triangle x, triangle y) {
  return triangle{{{x[0][0] - y[0][0], x[0][1] - y[0][1]},
                   {x[1][0] - y[1][0], x[1][1] - y[1][1]},
                   {x[2][0] - y[2][0], x[2][1] - y[2][1]}}};
}

inline double dist(double2 u, double2 v) {
  double dx = v[0] - u[0];
  double dy = v[1] - u[1];
  return sqrt(dx * dx + dy * dy);
}