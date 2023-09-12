#pragma once

#include <array>
#include <vector>

template < typename T >  
using vec = std::vector< T >;

using int2 = std::array<int,2>;
using int3 = std::array<int,3>;

using double2 = std::array<double,2>;
using double3 = std::array<double,3>;

using triangle = std::array<double2, 3>;
