#pragma once

#include "graph.hpp"
#include "cover.hpp"

size_t apply_rule_one(Graph&, Cover&);
void compute_common_neighbors(Graph const&, Cover const&, node_container_t const&, node_container_t&);
bool is_clique(Graph const&, Cover const&, node_container_t const&);
size_t apply_rule_two(Graph&, Cover&);
void compute_prisoners_and_exits(Graph const&, Cover const&, node_t, node_container_t&, node_container_t&);
bool prisoners_dominate_exits(Graph const&, Cover const&, node_container_t const&, node_container_t const&);
size_t apply_rule_three(Graph&, Cover&);
void apply_reductions(Graph&, Cover&);
void compute_all_maximal_cliques(Graph const&, Cover const&, node_container_t&, node_container_t&, node_container_t&, std::vector<node_container_t>&);
size_t compute_score(Graph const&, Cover const&, node_t v1, node_t v2);
void compute_edge_clique_cover(Graph&, Cover&);