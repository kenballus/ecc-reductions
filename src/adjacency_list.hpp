#pragma once

#include <unordered_set>
#include <unordered_map>
#include <cstdint>

typedef uint32_t node_t; // I might eventually need more than 4 billion nodes...
typedef std::unordered_set<node_t> node_container_t;
typedef std::unordered_map<node_t, node_container_t> adj_list_data_t;

class AdjacencyList {
public:
    adj_list_data_t data; // Ideally this wouldn't be public. I'll fix this later.

    AdjacencyList();

    void add_edge(node_t, node_t);
    void add_node(node_t);
    bool has_edge(node_t, node_t) const;
    bool has_node(node_t) const;
    node_container_t const& neighbors(node_t) const;
};