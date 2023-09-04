#!/bin/sh -v
CC=clang++
$CC -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread
$CC -c src/RNGs/other/*.cpp -O3 -Iinclude -pthread

ar rcs libPractRand.a *.o

$CC -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread
#$CC -o RNG_benchmark tools/RNG_benchmark.cpp libPractRand.a -O3 -Iinclude -pthread
#$CC -o RNG_output tools/RNG_output.cpp libPractRand.a -O3 -Iinclude -pthread

##nice -n 19 ./RNG_test rc4ok

