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
    bool exists(node_t v) const;
    void find_common_neighbors(node_t, node_t, std::unordered_set<node_t>&) const;
    bool is_clique(std::unordered_set<node_t> const&) const;
    bool has_same_neighbors(node_t v1, node_t v2) const;
};

std::ostream& operator<<(std::ostream&, Graph const&);