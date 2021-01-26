#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "graph.hpp"

Graph::Graph() {
    e = 0;
    n = 0;
}

Graph::Graph(std::string const& filename) {
    e = 0;
    n = 0;

    // I should probably validate this input
    std::ifstream ifs = std::ifstream(filename);
    assert(ifs.is_open());

    std::string line;

    std::unordered_map<node_t, std::unordered_set<node_t>> adj_list;

    while (std::getline(ifs, line)) {
        auto iss = std::istringstream(line);

        node_t v1;
        iss >> v1;
        node_t v2;
        iss >> v2;

        add_edge(v1, v2);
    }

    ifs.close();
}

void Graph::write_edge_list_file(std::string const& filename) const {
    auto ofs = std::ofstream(filename);
    assert(ofs.is_open());
    ofs << n << '\n';
    for (auto const& [v, neighbors] : adj_list) {
        for (auto neighbor : neighbors) {
            ofs << v << ' ' << neighbor << '\n';            
        }
    }
    ofs.close();
}

void Graph::add_node(node_t v) {
    assert(!adj_list.contains(v));

    adj_list.insert({v, {}});
    n++;
}

void Graph::add_edge(node_t v1, node_t v2) {
    if (!adj_list.contains(v1)) {
        add_node(v1);
    }
    if (!adj_list.contains(v2)) {
        add_node(v2);
    }
    adj_list.find(v1)->second.insert(v2);
    adj_list.find(v2)->second.insert(v1);
    e++;
}

bool Graph::has_edge(node_t v1, node_t v2) const {
    return neighbors(v1).contains(v2);
}

void Graph::find_prisoners_and_exits(node_t v1, std::unordered_set<node_t>& prisoners, std::unordered_set<node_t>& exits) const {
    assert(prisoners.empty() && exits.empty());

    for (node_t v2 : neighbors(v1)) {
        for (node_t v2_neighbor : neighbors(v2)) {
            if (v2_neighbor == v1) continue;

            if (!neighbors(v1).contains(v2_neighbor)) {
                exits.insert(v2);
                break;
            }
        }
        if (!exits.contains(v2)) {
            prisoners.insert(v2);
        }
    }
}

bool Graph::prisoners_dominate_exits(std::unordered_set<node_t> const& prisoners, std::unordered_set<node_t> const& exits) const {
    for (node_t v1 : exits) {
        bool dominated = false;
        for (node_t v2 : prisoners) {
            if (neighbors(v1).contains(v2)) {
                dominated = true;
                break;
            }
        }
        if (!dominated) {
            return false;
        }
    }

    return true;
}

void Graph::unlink_node(node_t v1) {
    e -= neighbors(v1).size();

    for (node_t v2 : neighbors(v1)) {
        adj_list.find(v2)->second.erase(v1);
    }
    adj_list.find(v1)->second.clear();
}

void Graph::delete_node(node_t v) {
    // Should be used in conjunction with unlink_node
    adj_list.erase(v);
    n--;
}

void Graph::remove_node(node_t v) {
    unlink_node(v);
    delete_node(v);
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

bool Graph::is_edge_clique_cover(std::vector<std::unordered_set<node_t>> const& ecc) const {
    return false; // to do
}

bool Graph::exists(node_t v) const {
    return !adj_list.find(v)->second.empty();
}

void Graph::remove_edge(node_t v1, node_t v2) {
    adj_list.find(v1)->second.erase(v2);
    adj_list.find(v2)->second.erase(v1);

    e--;
}

std::unordered_set<node_t> const& Graph::neighbors(node_t v) const {
    return adj_list.find(v)->second;
}

std::ostream& operator<<(std::ostream& os, Graph const& graph) {
    for (auto const& [v1, neighbors] : graph.adj_list) {
        os << v1 << ": ";
        for (node_t v2 : neighbors) {
            os << v2 << " ";
        }
        os << std::endl;
    }

    return os;
}