CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Werror -g -O0

OBJ_DIR := obj
_OBJ := adjacency_list.o graph.o ecc.o cover.o 
OBJ := $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

SRC_DIR := src

BINARY_NAME := ecc

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(BINARY_NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BINARY_NAME)
