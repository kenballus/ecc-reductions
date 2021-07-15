CPP := g++
CPPFLAGS := -std=c++20 -Wall -pedantic -O3 -march=native

OBJ_DIR := obj
_OBJ := adjacency_list.o graph.o ecc.o cover.o 
OBJ := $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

SRC_DIR := src

BINARY_NAME := ecc

# Needed so `make` defaults to `make ecc`
.DEFAULT_GOAL := $(BINARY_NAME)

# Compile all the object files except main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@ 

# Compile the target
$(BINARY_NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/main.cpp $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BINARY_NAME)
