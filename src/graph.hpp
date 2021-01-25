#pragma once

#include <unordered_set>
#include <vector>
#include <string>

typedef uint32_t node_t;

struct Graph {
    std::vector<std::unordered_set<node_t>> adj_list;
    size_t n;
    size_t e;

    Graph(size_t);
    Graph(std::string);

    void add_edge(node_t, node_t);
    void remove_edge(node_t, node_t);

    bool has_edge(node_t, node_t) const;
    std::unordered_set<node_t> const& neighbors(node_t) const;
    void remove_all_adjacent_edges(node_t);
};

std::ostream& operator<<(std::ostream&, Graph const&);