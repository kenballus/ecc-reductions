#include <utility>
#include <cassert>

#include "cover.hpp"
#include "graph.hpp"

size_t node_pair_hash::operator()(std::pair<uint32_t, uint32_t> const& p) const {
// Ensure that size_t, the type of the hash, is large enough
    assert(sizeof(size_t) >= sizeof(uint32_t) * 2);
    return (((size_t)p.first) << sizeof(uint32_t)) | ((size_t)p.second);
}

Cover::Cover() {
    size = 0;
}

bool Cover::is_covered(node_t v1, node_t v2) const {
    return covered_edges.contains({v1, v2});
}

void Cover::cover_edge(node_t v1, node_t v2) {
    if (!covered_edges.contains({v1, v2})) {
        covered_edges.insert({v1, v2});
    }
    if (!covered_edges.contains({v2, v1})) {
        covered_edges.insert({v2,v1});
    }
}

void Cover::shadow_node(node_t target, node_t shadow) {
    // Shadow will be inserted into any clique that target is in.
    shadows.push_back({target, shadow});
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

