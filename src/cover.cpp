#include <utility>
#include <cassert>

#include "cover.hpp"
#include "graph.hpp"

Cover::Cover() {
    size = 0;
}

bool Cover::is_covered(node_t v1, node_t v2) const {
    return covered_edges.contains(std::make_pair(v1, v2));
}

void Cover::cover_edge(node_t v1, node_t v2) {
    auto pair = std::make_pair(v1, v2);
    if (!covered_edges.contains(pair)) {
        covered_edges.insert(pair);
    }
    pair = std::make_pair(v2, v1);
    if (!covered_edges.contains(pair)) {
        covered_edges.insert(pair);
    }
}

void Cover::cover_clique(std::unordered_set<node_t> const& clique) {
    size++;
    bool is_new_clique = false;
    for (auto it1 = clique.begin(); it1 != clique.end(); it1++) {
        for (auto it2 = std::next(it1, 1); it2 != clique.end(); it2++) {
            if (!is_covered(*it1, *it2)) {
                is_new_clique = true;
            }

            cover_edge(*it1, *it2);
        }
    }

    assert(is_new_clique);
}