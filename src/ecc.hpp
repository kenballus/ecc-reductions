#pragma once

#include <vector>
#include <utility>

#include "cover.hpp"
#include "graph.hpp"


bool is_edge_clique_cover(Graph const&, Cover const&);
bool decompress_verify(Graph const&, Cover const&);
bool compute_edge_clique_cover(Graph const&, Cover&, size_t&, size_t&);