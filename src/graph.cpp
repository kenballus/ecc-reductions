#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

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
    adj_list[v1].insert(v2);
    adj_list[v2].insert(v1);
    e++;
}

bool Graph::has_edge(node_t v1, node_t v2) const {
    return neighbors(v1).contains(v2);
}

void Graph::find_prisoners_and_exits(node_t v1, std::unordered_set<node_t>& prisoners, std::unordered_set<node_t>& exits) {
    for (node_t v2 : neighbors(v1)) {
        for (node_t v2_neighbor : neighbors(v2)) {
            if (v2_neighbor == v1) continue;

            if (!neighbors(v1).contains(v2_neighbor)) {
                prisoners
            }
        }
    }
}

void Graph::remove_all_adjacent_edges(node_t v1) {
    e -= neighbors(v1).size();

    for (node_t v2 : neighbors(v1)) {
        adj_list[v2].erase(v1);
    }
    adj_list[v1].clear();
}

bool Graph::has_same_neighbors(node_t v1, node_t v2) const {
    for (node_t v : neighbors(v1)) {
        if (!neighbors(v2).contains(v)) {
            return false;
        }
    }
    // This is a little redundant, but I'm not sure what would be faster.
    for (node_t v : neighbors(v2)) {
        if (!neighbors(v1).contains(v)) {
            return false;
        }
    }

    return true;
}

void Graph::find_common_neighbors(node_t v1, node_t v2, std::unordered_set<node_t>& output) const {
    assert(output.empty());
    for (auto it = neighbors(v1).begin(); it != neighbors(v1).end(); it++) {
        if (neighbors(v2).contains(*it)) {
            output.insert(*it);
        }
    }
}

bool Graph::is_clique(std::unordered_set<node_t> const& nodes) const {
    for (auto it1 = nodes.begin(); it1 != nodes.end(); it1++) {
        for (auto it2 = std::next(it1, 1); it2 != nodes.end(); it2++) {
            if (!neighbors(*it2).contains(*it1)) {
                return false;
            }
        }
    }

    return true;
}

bool Graph::exists(node_t v) const {
    return !adj_list[v].empty();
}

void Graph::remove_edge(node_t v1, node_t v2) {
    size_t result = adj_list[v1].erase(v2) + adj_list[v2].erase(v1);

    e--;

    assert(result);
}

std::unordered_set<node_t> const& Graph::neighbors(node_t v) const {
    return adj_list[v];
}

std::ostream& operator<<(std::ostream& os, Graph const& g) {
    for (node_t i = 0; i < g.adj_list.size(); i++) {
        os << i << ": ";
        for (node_t j : g.adj_list[i]) {
            os << j << " ";
        }
        os << std::endl;
    }

    return os;
}