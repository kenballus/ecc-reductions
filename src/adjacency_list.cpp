#include "adjacency_list.hpp"

// All this find->second business needs to get cleaned up too.

AdjacencyList::AdjacencyList() {}

void AdjacencyList::add_edge(node_t v1, node_t v2) {
    if (not has_edge(v1, v2)) {
        data.find(v1)->second.insert(v2);
        data.find(v2)->second.insert(v1);
    }
}

void AdjacencyList::add_node(node_t v) {
    data.insert({v, {}});
}

bool AdjacencyList::has_edge(node_t v1, node_t v2) const {
    return data.find(v1)->second.contains(v2);
}

bool AdjacencyList::has_node(node_t v) const {
    return data.contains(v);
}

node_container_t const& AdjacencyList::neighbors(node_t v) const {
    return data.find(v)->second;
}