cmake_minimum_required(VERSION 3.16)
project(mesh_coarsening LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB gc_sources ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(mesh_coarsening SHARED ${gc_sources})
target_include_directories(mesh_coarsening PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(mesh_coarsening PUBLIC "-DDATA_DIR=\"${PROJECT_SOURCE_DIR}/data/\"")

# prevent CMake error on macs: 
# Could NOT find Threads (missing: Threads_FOUND)
IF(APPLE)
  set(CMAKE_THREAD_LIBS_INIT "-lpthread")
  set(CMAKE_HAVE_THREADS_LIBRARY 1)
  set(CMAKE_USE_WIN32_THREADS_INIT 0)
  set(CMAKE_USE_PTHREADS_INIT 1)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

if (MC_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

if (MC_ENABLE_ASAN)
  add_link_options(-fsanitize=address)
  add_compile_options(-fsanitize=address)
endif()
