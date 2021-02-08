#pragma once

#include <unordered_set>
#include <vector>
#include <string>

#include "adjacency_list.hpp"

class Graph {
private:
    AdjacencyList adj_list;

public:
    size_t n;
    size_t e;

    Graph();
    Graph(std::string const&);

    void write_edge_list_file(std::string const&) const;

    adj_list_data_t const& get_adj_list() const;
    // Ideally, there would be an iterable for nodes and an iterable for edges.
    // I don't know how to do this yet.

    // These methods do precondition checks, then call corresponding methods in AdjacencyList.
    void add_edge(node_t, node_t);
    void add_node(node_t);
    bool has_edge(node_t, node_t) const;
    bool has_node(node_t v) const;
    node_container_t const& neighbors(node_t) const;
};