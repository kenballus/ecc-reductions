#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

typedef uint32_t node_t; // I might eventually need more than 4 billion nodes...

struct Graph {
    std::unordered_map<node_t, std::unordered_set<node_t>> adj_list;
    size_t n;
    size_t e;

    Graph();
    Graph(std::string const&);

    void add_edge(node_t, node_t);
    void remove_edge(node_t, node_t);
    void unlink_node(node_t);
    void delete_node(node_t);
    void remove_node(node_t);
    void add_node(node_t);

    bool has_edge(node_t, node_t) const;
    std::unordered_set<node_t> const& neighbors(node_t) const;
    bool exists(node_t v) const;
    void find_common_neighbors(node_t, node_t, std::unordered_set<node_t>&) const;
    bool is_clique(std::unordered_set<node_t> const&) const;
    bool has_same_neighbors(node_t, node_t) const;
    void find_prisoners_and_exits(node_t, std::unordered_set<node_t>&, std::unordered_set<node_t>&) const;
    bool is_edge_clique_cover(std::vector<std::unordered_set<node_t>> const&) const;
    bool prisoners_dominate_exits(std::unordered_set<node_t> const&, std::unordered_set<node_t> const&) const;

    void write_edge_list_file(std::string const&) const;
};

std::ostream& operator<<(std::ostream&, Graph const&);