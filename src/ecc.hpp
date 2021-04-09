#pragma once

#include <vector>
#include <utility>

#include "cover.hpp"
#include "graph.hpp"

size_t apply_rule_one(Graph const&, Cover&);
void compute_common_neighbors(Graph const&, Cover const&, node_t, node_t, node_container_t&);
bool is_clique(Graph const&, Cover const&, node_container_t const&);
size_t apply_rule_two(Graph const&, Cover&);
void compute_prisoners_and_exits(Graph const&, Cover const&, node_t, node_container_t&, node_container_t&);
bool prisoners_dominate_exits(Graph const&, node_container_t const&, node_container_t const&);
size_t apply_rule_three(Graph const&, Cover&);
bool is_edge_clique_cover(Graph const&, Cover const&);
bool decompress_verify(Graph const&, Cover const&);
void apply_reductions(Graph const&, Cover&);
void compute_all_maximal_cliques(Graph const&, Cover const&, node_container_t&, node_container_t&, node_container_t&, std::vector<node_container_t>&);
size_t compute_score(Graph const&, Cover const&, node_t, node_t);
std::pair<node_t, node_t> const pick_lowest_score_edge(Graph const&, Cover const&);
std::pair<node_t, node_t> const pick_highest_score_edge(Graph const&, Cover const&);
std::pair<node_t, node_t> const pick_first_edge(Graph const&, Cover const&);
void print_reduced_graph(Graph const&, Cover const&);
void extract_irreducible_subgraph(Graph const&, Cover&);
size_t size_of_big_vertex_independent_set(Graph const&, Cover const&);
size_t size_of_big_edge_independent_set(Graph const&, Cover const&);
bool compute_edge_clique_cover(Graph const&, Cover&, size_t const, size_t&);