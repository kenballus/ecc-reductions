#pragma once

#include <vector>
#include <utility>

#include "cover.hpp"
#include "graph.hpp"

bool is_edge_clique_cover(Graph const& graph, Cover const& cover);
bool decompress_verify(Graph const& graph, Cover const& cover);
bool compute_edge_clique_cover(Graph const& graph, Cover& cover, size_t const k, size_t& total_calls);
void order_neighbors_by_degree(Graph& graph);
void order_neighbors_by_neighbor_degree(Graph& graph);