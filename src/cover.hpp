#pragma once

#include <utility> // for pair
#include <unordered_set>
#include <cassert>

#include "graph.hpp"

// CITE: https://stackoverflow.com/questions/15160889/how-can-i-make-an-unordered-set-of-pairs-of-integers-in-c
struct node_pair_hash {
  size_t operator()(std::pair<uint32_t, uint32_t> const& p) const {
    //Ensure that std::size_t, the type of the hash, is large enough
    assert(sizeof(size_t) >= sizeof(uint32_t) * 2);
    return (((size_t)p.first) << sizeof(uint32_t)) | ((size_t)p.second);
  }
};

struct Cover {
    node_t size;
    std::unordered_set<std::pair<node_t, node_t>, node_pair_hash> covered_edges;

    Cover();

    bool is_covered(node_t, node_t) const;
    void cover_edge(node_t, node_t);
    void cover_clique(std::unordered_set<node_t> const&);
};