#!/bin/bash

# This benchmarks
# the full program given a varaiety of threads

cd ./executables
make

echo ""

echo "Benchmarking with 2 threads:"
export export OMP_NUM_THREADS=2
time ./project

echo ""

echo "Benchmarking with 3 threads:"
export export OMP_NUM_THREADS=3
time ./project

echo ""

echo "Benchmarking with 4 threads:"
export export OMP_NUM_THREADS=4
time ./project

echo ""

echo "Benchmarking with 5 threads:"
export export OMP_NUM_THREADS=5
time ./project

echo ""

echo "Benchmarking with 6 threads:"
export export OMP_NUM_THREADS=6
time ./project

echo ""

echo "Benchmarking with 7 threads:"
export export OMP_NUM_THREADS=7
time ./project

echo ""

echo "Benchmarking with 8 threads:"
export export OMP_NUM_THREADS=8
time ./project
