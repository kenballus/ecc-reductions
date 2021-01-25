#include <utility>

#include "cover.hpp"
#include "graph.hpp"

Cover::Cover() {
    size = 0;
}

bool Cover::is_covered(node_t v1, node_t v2) const {
    return edges.contains(std::make_pair(v1, v2));
}