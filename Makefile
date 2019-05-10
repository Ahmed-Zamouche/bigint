

all: TEST_bigint

ifeq ($(OS), Windows_NT)
 RM := del
else
 RM := rm
endif

#OPT := -Og -ggdb
OPT := -O0 -g
#OPT := -O3


#CXX := clang++ -target x86_64-pc-windows-gnu
CXX := g++

CFLAGS := $(OPT) -Wall --std=c++17 -Wextra -pedantic\

CXXFLAGS := $(CFLAGS)

LFLAGS := 

TEST_run:
	powershell  ./test_runner.ps1

TEST_bigint: bigint TEST_bigint.cpp
	$(CXX) $(CXXFLAGS) $(LFLAGS) .\bigint.o .\TEST_bigint.cpp -o .\TEST_bigint
TEST_debug: TEST_bigint
	gdb .\TEST_bigint

bigint: bigint.cpp bigint.hpp
	$(CXX) $(CXXFLAGS) -c .\bigint.cpp -o .\bigint.o

clean:
	$(RM) *.o
	$(RM) TEST_bigint.exe