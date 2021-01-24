#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "graph.hpp"

Graph::Graph(size_t arg_n) {
    n = arg_n;
    e = 0;

    for (size_t i = 0; i < n; ++i) {
        adj_list.push_back(new std::unordered_set<size_t>);
    }
}

Graph::Graph(std::string filename) {
    e = 0;

    // I should probably validate this input
    std::ifstream el_ifstream = std::ifstream(filename);

    assert(el_ifstream.is_open());

    std::string line;
    assert(std::getline(el_ifstream, line));

    {
        std::istringstream iss(line);
        iss >> n;
    }

    for (size_t i = 0; i < n; ++i) {
        adj_list.push_back(new std::unordered_set<size_t>);
    }

    while (std::getline(el_ifstream, line)) {
        std::istringstream iss(line);

        size_t v1;
        iss >> v1;
        size_t v2;
        iss >> v2;

        add_edge(v1, v2);
    }

    el_ifstream.close();
}

Graph::~Graph() {
    for (size_t i = 0; i < n; ++i) {
        delete adj_list[i];
    }
}

void Graph::add_edge(size_t v1, size_t v2) {
    assert(v1 < n && v2 < n);

    adj_list[v1]->insert(v2);
    adj_list[v2]->insert(v1);
    e++;
}

bool Graph::has_edge(size_t v1, size_t v2) const {
    return adj_list[v1]->contains(v2);
}

void Graph::remove_edge(size_t v1, size_t v2) {
    adj_list[v1]->erase(v2);
    adj_list[v2]->erase(v1);
}

std::ostream& operator<<(std::ostream& os, Graph const& g) {
    for (size_t i = 0; i < g.adj_list.size(); ++i) {
        os << i << ": ";
        for (size_t j : *(g.adj_list[i])) {
            os << j << " ";
        }
        os << std::endl;
    }

    return os;
}

int main() {
    Graph g = Graph("test_graph.el");
    std::cout << g << std::flush;
    return EXIT_SUCCESS;
}