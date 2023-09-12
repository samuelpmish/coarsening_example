## Build instructions

clone repo
```
git clone git@github.com:samuelpmish/coarsening_example.git
```

configure cmake
```
cd coarsening_example
cmake . -Bbuild -DCMAKE_BUILD_TYPE=Release -DMC_ENABLE_TESTING=True
```

build
```
cd build && make -j
```

run test
```
cd tests
ctest

Test project /Users/sam/code/coarsening_example/build/tests
    Start 1: coarsen_tests
1/7 Test #1: coarsen_tests ....................   Passed    0.01 sec
    Start 2: coloring_tests
2/7 Test #2: coloring_tests ...................   Passed    0.00 sec
    Start 3: find_corners_tests
3/7 Test #3: find_corners_tests ...............   Passed    0.00 sec
    Start 4: find_edges_tests
4/7 Test #4: find_edges_tests .................   Passed    0.00 sec
    Start 5: find_islands_tests
5/7 Test #5: find_islands_tests ...............   Passed    0.00 sec
    Start 6: find_quads_tests
6/7 Test #6: find_quads_tests .................   Passed    0.00 sec
    Start 7: improve_connectivity_tests
7/7 Test #7: improve_connectivity_tests .......   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7
```
