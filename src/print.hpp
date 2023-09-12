#pragma once

#include <array>
#include <vector>
#include <iostream>

template < typename T >
std::ostream & operator<<(std::ostream& out, std::vector< T > vec) {
  size_t n = vec.size();
  out << "{";
  for (int i = 0; i < n; i++) {
    out << vec[i];
    if (i < n-1) out << ", ";
  }
  out << "}";
  return out;
}

template < typename T, size_t n >
std::ostream & operator<<(std::ostream& out, std::array< T, n > arr) {
  out << "{";
  for (int i = 0; i < n; i++) {
    out << arr[i];
    if (i < n-1) out << ", ";
  }
  out << "}";
  return out;
}