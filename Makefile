CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Werror -g

graph: graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?
