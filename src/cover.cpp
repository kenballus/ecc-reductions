#include <cassert>
#include <utility>

#include "cover.hpp"
#include "graph.hpp"

size_t NodePairHash::operator()(std::pair<uint32_t, uint32_t> const& p) const {
    // Ensure that size_t, the type of the hash, is large enough
    assert(sizeof(size_t) >= sizeof(uint32_t) * 2);
    return (((size_t)p.first) << sizeof(uint32_t)) | (size_t)p.second;
}

Cover::Cover() {}

bool Cover::is_covered(node_t v1, node_t v2) const {
    return covered_edges.contains({v1, v2});
}

void Cover::remove_node(node_t v) {
    removed_nodes.insert(v);
}

void Cover::cover_edge(node_t v1, node_t v2) {
    if (not covered_edges.contains({v1, v2})) {
        covered_edges.insert({v1, v2});
    }
    if (not covered_edges.contains({v2, v1})) {
        covered_edges.insert({v2,v1});
    }
}

void Cover::shadow_node(node_t target, node_t shadow) {
    // Shadow will be inserted into any clique that target is in.
    if (not shadows.contains(target)) {
        shadows.insert({target, {}});
    }
    shadows.find(target)->second.insert(shadow);
}

void Cover::unshadow() {
    // This should be called after the reduced instance of the problem has been solved.
    for (node_container_t& clique : cliques) {
        std::vector<node_t> to_add;
        for (auto& [target, shadow_set] : shadows) {
            if (clique.contains(target)) { // If the target is in this clique,
                to_add.insert(to_add.end(), shadow_set.begin(), shadow_set.end()); // put its shadows in there too.
            }
        }

        for (size_t i = 0; i < to_add.size(); i++) {
            if (shadows.contains(to_add[i])) { // If the things we just added have shadows,
                node_container_t shadow_set = shadows.find(to_add[i])->second;
                to_add.insert(to_add.end(), shadow_set.begin(), shadow_set.end()); // don't forget about them.
            }
        }

        for (node_t v : to_add) {
            clique.insert(v);
        }
    }
}

void Cover::cover_clique(node_container_t const& clique) {
    bool is_new_clique = false;
    for (auto it1 = clique.cbegin(); it1 != clique.cend(); it1++) {
        for (auto it2 = std::next(it1, 1); it2 != clique.cend(); it2++) {
            if (not is_covered(*it1, *it2)) {
                is_new_clique = true;
                cover_edge(*it1, *it2);
            }
        }
    }

    assert(is_new_clique);
    cliques.push_back(clique);
}

bool Cover::is_removed(node_t v) const {
    return removed_nodes.contains(v);
}

size_t Cover::num_covered_edges() const {
    return covered_edges.size() / 2;
}