echo off

rem g++ -O3 -Wall --std=c++11 -c .\bigint.cpp -o .\bigint.o

rem g++ -O3 -Wall --std=c++11 .\bigint.o .\TEST_bigint.cpp -o .\TEST_bigint

.\TEST_bigint > result.js

node result.js