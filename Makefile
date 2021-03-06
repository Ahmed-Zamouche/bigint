

all: TEST_bigint

ifeq ($(OS), Windows_NT)
 RM := del
else
 RM := rm
endif

#OPT := -Og -ggdb
OPT := -O3


#CXX := clang++ -target x86_64-pc-windows-gnu
CXX := g++

CFLAGS := $(OPT) -Wall --std=c++11 \
#-fsanitize=undefined -fsanitize=address

CXXFLAGS := $(CFLAGS)

LFLAGS := 

TEST_bigint: bigint TEST_bigint.cpp
	$(CXX) $(CXXFLAGS) $(LFLAGS) .\bigint.o .\TEST_bigint.cpp -o .\TEST_bigint.exe
TEST_debug: TEST_bigint
	gdb .\TEST_bigint.exe	

bigint: bigint.cpp bigint.hpp
	$(CXX) $(CXXFLAGS) -c .\bigint.cpp -o .\bigint.o

test: 
	@node tester.js "$(shell .\TEST_bigint)"
clean:
	$(RM) *.o
	$(RM) TEST_bigint.exe