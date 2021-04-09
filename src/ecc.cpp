#include <iostream>
#include <vector>
#include <utility>
// #include <cassert>
#include <limits>
#include <unordered_set>

#include "ecc.hpp"
#include "cover.hpp"
#include "graph.hpp"

size_t apply_rule_one(Graph const& graph, Cover& cover) {
    size_t ret = 0;

    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;

        bool all_edges_covered = true;
        for (node_t v2 : v1_neighbors) {
            if (cover.is_removed(v2)) continue;

            if (not cover.is_covered(v1, v2)) {
                all_edges_covered = false;
                break;
            }
        }

        if (all_edges_covered) {
            cover.remove_node(v1);
            for (node_t neighbor : graph.neighbors(v1)) {
                cover.cover_edge(neighbor, v1);
            }
            ret++;
        }
    }

    return ret;
}

void compute_common_neighbors(Graph const& graph, Cover const& cover, node_t v1, node_t v2, node_container_t& output) {
    // assert(output.empty());

    // Assumes nodes contains only valid nodes (not removed)

    for (node_t neighbor : graph.neighbors(v1)) {
        if (cover.is_removed(neighbor) or neighbor == v2) continue;
        if (graph.has_edge(v2, neighbor)) {
            output.insert(neighbor);
        }
    }
}

bool is_clique(Graph const& graph, Cover const& cover, node_container_t const& nodes) {
    for (auto it1 = nodes.cbegin(); it1 != nodes.cend(); it1++) {
        if (cover.is_removed(*it1)) continue;
        for (auto it2 = std::next(it1, 1); it2 != nodes.cend(); it2++) {
            if (cover.is_removed(*it2)) continue;
            if (not graph.has_edge(*it1, *it2)) return false;
        }
    }

    return true;
}

size_t apply_rule_two(Graph const& graph, Cover& cover) {
    size_t ret = 0;

    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;
        for (node_t v2 : v1_neighbors) {
            if (v2 <= v1 or cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;

            node_container_t common_neighbors;
            compute_common_neighbors(graph, cover, v1, v2, common_neighbors);
            common_neighbors.insert(v1);
            common_neighbors.insert(v2);

            if (is_clique(graph, cover, common_neighbors)) {
                cover.cover_clique(common_neighbors);
                ret++;
            }
        }
    }

    return ret;
}

void compute_prisoners_and_exits(Graph const& graph, Cover const& cover, node_t v1, node_container_t& prisoners, node_container_t& exits) {
    // assert(prisoners.empty());
    // assert(exits.empty());

    for (node_t v2 : graph.neighbors(v1)) {
        if (cover.is_removed(v2)) continue;

        for (node_t v2_neighbor : graph.neighbors(v2)) {
            if (v2_neighbor == v1 or cover.is_removed(v2_neighbor)) continue;

            if (not graph.has_edge(v1, v2_neighbor)) {
                exits.insert(v2);
                break;
            }
        }
        if (not exits.contains(v2)) {
            prisoners.insert(v2);
        }
    }
}

bool prisoners_dominate_exits(Graph const& graph, Cover const& cover, node_container_t const& prisoners, node_container_t const& exits) {
    // Assumes prisoners and exits all exist.
    if (prisoners.empty()) return false; // This is something to think about harder.
    if (exits.empty()) return true;

    for (node_t exit : exits) {
        bool dominated = false;
        for (node_t prisoner : prisoners) { // It might be better to loop over the neighbors of the exit.
            if (graph.has_edge(exit, prisoner) and not cover.is_covered(exit, prisoner)) { // This is beyond what the paper said. For justification, try on graphs/easy/right.graph
                dominated = true;
                break;
            }
        }
        if (not dominated) {
            return false;
        }
    }

    return true;
}

size_t apply_rule_three(Graph const& graph, Cover& cover) {
    // Assumes full reduction wrt rules 1 and 2.

    for (auto const& [v, _] : graph.get_adj_list()) {
        if (cover.is_removed(v)) continue;

        node_container_t prisoners;
        node_container_t exits;
        compute_prisoners_and_exits(graph, cover, v, prisoners, exits);

        // Unnecessary if graph is already reduced with rules 1 and 2:
        for (node_t const& prisoner : prisoners) {
            bool found_valid_nbr = false;
            for (auto const& nbr_of_prisoner : graph.neighbors(prisoner)) {
                if (nbr_of_prisoner != v and not cover.is_removed(nbr_of_prisoner) and not cover.is_covered(nbr_of_prisoner, prisoner)) {
                    found_valid_nbr = true;
                    break;
                }
            }
            if (not found_valid_nbr) return 0;
        }

        if (prisoners_dominate_exits(graph, cover, prisoners, exits)) {
            for (node_t prisoner : prisoners) {
                cover.shadow_node(prisoner, v);
            }
            cover.remove_node(v);
            for (node_t neighbor : graph.neighbors(v)) {
                cover.cover_edge(v, neighbor);
            }
            return true;
        }
    }

    return false;
}

bool is_edge_clique_cover(Graph const& graph, Cover const& cover) {
    return cover.num_covered_edges() == graph.e;
}

bool decompress_verify(Graph const& original, Cover const& cover) {
    Graph decompressed;

    for (node_container_t const& clique: cover.cliques) {
        for (node_t v1 : clique) {
            if (not decompressed.has_node(v1)) {
                decompressed.add_node(v1);
            }

            for (node_t v2 : clique) {
                if (v1 == v2) continue;
                if (not decompressed.has_node(v2)) {
                    decompressed.add_node(v2);
                }
                decompressed.add_edge(v1, v2);
            }
        }
    }

    for (auto const& [v, v_neighbors] : original.get_adj_list()) {
        if (not decompressed.has_node(v)) {
            return false;
        }
        std::unordered_set<node_t> neighbors;
        neighbors.insert(v_neighbors.begin(), v_neighbors.end());
        neighbors.insert(decompressed.neighbors(v).begin(), decompressed.neighbors(v).end());
        if (neighbors.size() != v_neighbors.size() or neighbors.size() != decompressed.neighbors(v).size()) {
            return false;
        }
    }

    return true;
}

void apply_reductions(Graph const& graph, Cover& cover) {
    while (true) {
        if (apply_rule_one(graph, cover)) continue;
        if (apply_rule_two(graph, cover)) continue;
        if (apply_rule_three(graph, cover)) continue;
        return;
    }
}

void compute_all_maximal_cliques(Graph const& graph, Cover const& cover, node_container_t& R, node_container_t& P, node_container_t& X, std::vector<node_container_t>& cliques) {
    // This is just Bron Kerbosch
    if (P.empty() and X.empty()) {
        cliques.push_back(R);
        return;
    }

    while (not P.empty()) {
        node_t const v = *P.cbegin();
        node_container_t new_R = R;
        new_R.insert(v);
        node_container_t new_P;
        node_container_t new_X;
        for (node_t n : graph.neighbors(v)) {
            if (cover.is_removed(n)) continue;
            if (P.contains(n)) {
                new_P.insert(n);
            }
            if (X.contains(n)) {
                new_X.insert(n);
            }
        }
        compute_all_maximal_cliques(graph, cover, new_R, new_P, new_X, cliques);
        P.erase(v);
        X.insert(v);
    }
}

size_t compute_score(Graph const& graph, Cover const& cover, node_t v1, node_t v2) {
    // assert(graph.has_edge(v1, v2));

    size_t result = 0;
    node_container_t common_neighbors;
    compute_common_neighbors(graph, cover, v1, v2, common_neighbors);

    for (auto it1 = common_neighbors.cbegin(); it1 != common_neighbors.cend(); it1++) {
        if (cover.is_removed(*it1)) continue;
        for (auto it2 = std::next(it1, 1); it2 != common_neighbors.cend(); it2++) {
            if (cover.is_removed(*it2)) continue;
            result += not graph.has_edge(*it1, *it2);
        }
    }

    return result;
}

std::pair<node_t, node_t> const pick_lowest_score_edge(Graph const& graph, Cover const& cover) {
    // Pick the lowest score edge. (The edge whose common neighbors are closest to forming a clique)
    size_t best_score = std::numeric_limits<size_t>::max();
    std::pair<node_t, node_t> best_edge = {std::numeric_limits<node_t>::max(), std::numeric_limits<node_t>::max()};

    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;
        for (node_t v2 : v1_neighbors) {
            if (v2 <= v1 or cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;
            size_t score = compute_score(graph, cover, v1, v2);
            if (score < best_score) {
                best_score = score;
                best_edge = {v1, v2};
            }
        }
    }

    return best_edge;
}

std::pair<node_t, node_t> const pick_highest_score_edge(Graph const& graph, Cover const& cover) {
    // Pick the lowest score edge. (The edge whose common neighbors are closest to forming a clique)
    size_t best_score = 0;
    std::pair<node_t, node_t> best_edge = {std::numeric_limits<node_t>::max(), std::numeric_limits<node_t>::max()};

    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;
        for (node_t v2 : v1_neighbors) {
            if (v2 <= v1 or cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;
            size_t score = compute_score(graph, cover, v1, v2);
            if (score > best_score) {
                best_score = score;
                best_edge = {v1, v2};
            }
        }
    }

    return best_edge;
}

std::pair<node_t, node_t> const pick_first_edge(Graph const& graph, Cover const& cover) {
    // Pick the first edge.
    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;
        for (node_t v2 : v1_neighbors) {
            if (cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;
            return {v1, v2};
        }
    }
    return {std::numeric_limits<node_t>::max(), std::numeric_limits<node_t>::max()};
}

void print_reduced_graph(Graph const& graph, Cover const& cover) {
    // For debugging
    for (auto const& [n, neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(n)) continue;
        for (auto const& neighbor : neighbors) {
            if (n > neighbor or cover.is_removed(neighbor)) continue;
            if (cover.is_covered(n, neighbor)) {
                std::cout << "#color=red\n";
            }
            std::cout << n << " " << neighbor << "\n";
        }
    }
}

void extract_irreducible_subgraph(Graph const& graph, Cover& cover) {
    // Prints out all the edges in G that aren't covered by a single round of reductions.

    apply_reductions(graph, cover);
    for (auto const& [n1, neighbors] : graph.get_adj_list()) {
        for (node_t n2 : neighbors) {
            if (cover.is_covered(n1, n2)) {
                std::cout << n1 << " " << n2 << "\n";
            }
        }
    }
}

size_t size_of_big_vertex_independent_set(Graph const& graph, Cover const& cover) {
    // Finds a large independent set among the uncovered vertices in the graph.

    std::vector<node_t> ind_set;
    for (auto const& [n1, _] : graph.get_adj_list()) {
        if (cover.is_removed(n1)) continue;
        bool independent = true;
        for (auto const& n2 : ind_set) {
            // Don't need to check if removed here, because we only add valid nodes to ind_set.
            if (graph.has_edge(n1, n2)) {
                independent = false;
                break;
            }
        }
        if (independent) ind_set.push_back(n1);
    }
    return ind_set.size();
}

size_t size_of_big_edge_independent_set(Graph const& graph, Cover const& cover) {
    // Computes a large set of edges such that the subgraph induced by their vertices is K_4-free.

    std::vector<std::pair<node_t, node_t>> ind_set;
    for (auto const& [n1, neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(n1)) continue;
        bool independent = true;
        for (auto const& n2 : neighbors) {
            if (n1 >= n2 or cover.is_removed(n2) or cover.is_covered(n1, n2)) continue;
            for (auto const& [v1, v2] : ind_set) {
                if (n1 == v1 or n2 == v2 or n2 == v1 or n1 == v2 or (graph.has_edge(n1, v1) and graph.has_edge(n2, v1) and graph.has_edge(n1, v2) and graph.has_edge(n2, v2))) {
                    independent = false;
                    break;
                }
            }
            if (independent) {
                ind_set.push_back({n1, n2});
                break;
            }
        }
    }
    return ind_set.size();
}

bool compute_edge_clique_cover(Graph const& graph, Cover& cover, size_t& k, size_t& total_calls) {
    total_calls++;

    apply_reductions(graph, cover);
    if (cover.cliques.size() >= k or cover.cliques.size() + std::max(size_of_big_edge_independent_set(graph, cover), size_of_big_vertex_independent_set(graph, cover)) >= k) return false;
    if (is_edge_clique_cover(graph, cover)) return true;

    std::pair<node_t, node_t> const best_edge = pick_lowest_score_edge(graph, cover);

    if (best_edge.first == std::numeric_limits<node_t>::max()) {
        std::cerr << "Best edge never got set. Probably every edge is deleted.\n";
        exit(1);
    }

    // Make R, P, and X from Bron Kerbosch
    // R contains vertices that have to be in the clique (for us, v1 and v2)
    // P contains vertices that could be in the clique (for us, their common neighbors)
    // X contains the clique as it is built. It's initialized empty.

    node_container_t common_neighbors;
    compute_common_neighbors(graph, cover, best_edge.first, best_edge.second, common_neighbors);
    node_container_t R;
    R.insert(best_edge.first);
    R.insert(best_edge.second);
    node_container_t X;
    std::vector<node_container_t> cliques;
    compute_all_maximal_cliques(graph, cover, R, common_neighbors, X, cliques);
    // assert(not cliques.empty());

    Cover best_cover = Cover(graph.n);
    bool found_better_cover = false;
    for (node_container_t const& clique : cliques) {
        Cover new_cover = cover; // a copy, on purpose
        new_cover.cover_clique(clique);

        if (compute_edge_clique_cover(graph, new_cover, k, total_calls) and new_cover.cliques.size() < k) {
            k = new_cover.cliques.size();
            best_cover = std::move(new_cover);
            found_better_cover = true;
        }
    }

    if (found_better_cover) {
        cover = std::move(best_cover);
        return true;
    }
    return false;
}
