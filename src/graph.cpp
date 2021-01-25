#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "graph.hpp"

Graph::Graph(size_t arg_n) {
    n = arg_n;
    e = 0;

    adj_list = std::vector<std::unordered_set<node_t>>(n);
}

Graph::Graph(std::string filename) {
    e = 0;

    // I should probably validate this input
    std::ifstream el_ifstream = std::ifstream(filename);

    assert(el_ifstream.is_open());

    std::string line;
    assert(std::getline(el_ifstream, line));

    std::istringstream iss = std::istringstream(line);
    iss >> n;

    adj_list = std::vector<std::unordered_set<node_t>>(n);

    while (std::getline(el_ifstream, line)) {
        iss = std::istringstream(line);

        node_t v1;
        iss >> v1;
        node_t v2;
        iss >> v2;

        add_edge(v1, v2);
    }

    el_ifstream.close();
}

void Graph::add_edge(node_t v1, node_t v2) {
    assert(v1 < n && v2 < n);

    adj_list[v1].insert(v2);
    adj_list[v2].insert(v1);
    e++;
}

bool Graph::has_edge(node_t v1, node_t v2) const {
    return adj_list[v1].contains(v2);
}

void Graph::remove_all_adjacent_edges(node_t v1) {
    adj_list[v1].erase(adj_list[v1].begin(), adj_list[v1].end());
}

void Graph::remove_edge(node_t v1, node_t v2) {
    adj_list[v1].erase(v2);
    adj_list[v2].erase(v1);
}

std::unordered_set<node_t> const& Graph::neighbors(node_t v) const {
    return adj_list[v];
}

std::ostream& operator<<(std::ostream& os, Graph const& g) {
    for (node_t i = 0; i < g.adj_list.size(); ++i) {
        os << i << ": ";
        for (node_t j : g.adj_list[i]) {
            os << j << " ";
        }
        os << std::endl;
    }

    return os;
}