#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <cassert>

#include "ecc.hpp"
#include "cover.hpp"
#include "graph.hpp"
#include "timer.hpp"

template<typename T>
void print_unordered_set(std::unordered_set<T> const& set) {
    for (T const& thing : set) {
        std::cerr << thing << ' ';
    }
    std::cerr << std::endl;
}

size_t apply_rule_one(Graph& graph, Cover& cover) {
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
            std::cerr << "Rule 1 is removing " << v1 << std::endl;
            ret++;
        }
    }

    return ret;
}

void compute_common_neighbors(Graph const& graph, Cover const& cover, node_container_t const& nodes, node_container_t& output) {
    assert(output.empty());
    if (nodes.empty()) return;

    // Assumes nodes contains only valid nodes (not removed)

    node_t lowest_degree_node = *nodes.cbegin(); // Named this way for a future optimization.

    for (node_t neighbor : graph.neighbors(lowest_degree_node)) {
        if (cover.is_removed(neighbor)) continue;
        bool adding = true;
        for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
            if (not graph.has_edge(*it, neighbor)) {
                adding = false;
                break;
            }
        }
        if (adding) {
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

size_t apply_rule_two(Graph& graph, Cover& cover) {
    size_t ret = 0;

    for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
        if (cover.is_removed(v1)) continue;
        for (node_t v2 : v1_neighbors) {
            if (v2 <= v1 or cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;

            node_container_t common_neighbors;
            compute_common_neighbors(graph, cover, {v1, v2}, common_neighbors);
            common_neighbors.insert(v1);
            common_neighbors.insert(v2);

            if (is_clique(graph, cover, common_neighbors)) {
                std::cerr << "Rule 2 is adding a clique: ";
                print_unordered_set(common_neighbors);
                cover.cover_clique(common_neighbors);
                ret++;
            }
        }
    }

    return ret;
}

void compute_prisoners_and_exits(Graph const& graph, Cover const& cover, node_t v1, node_container_t& prisoners, node_container_t& exits) {
    assert(prisoners.empty());
    assert(exits.empty());

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

bool prisoners_dominate_exits(Graph const& graph, node_container_t const& prisoners, node_container_t const& exits) {
    // Assumes prisoners and exits all exist.
    for (node_t exit : exits) {
        bool dominated = false;
        for (node_t prisoner : prisoners) {
            if (graph.has_edge(exit, prisoner)) {
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
size_t apply_rule_three(Graph& graph, Cover& cover) {
    // Assumes full reduction wrt rules 1 and 2.
    size_t ret = 0;

    for (auto const& [v, _] : graph.get_adj_list()) {
        node_container_t prisoners;
        node_container_t exits;
        compute_prisoners_and_exits(graph, cover, v, prisoners, exits);

        if (not prisoners.empty() and prisoners_dominate_exits(graph, prisoners, exits)) {
            for (node_t prisoner : prisoners) {
                cover.shadow_node(prisoner, v);
            }
            cover.remove_node(v);
            std::cerr << "Rule 3 is removing node " << v << std::endl;
            ret++;
        }
    }

    return ret;
}

bool is_edge_clique_cover(Graph const& original, Cover const& cover) {
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

    if (original.get_adj_list().empty()) {
        return decompressed.get_adj_list().empty();
    }
    for (auto const& [v, v_neighbors] : original.get_adj_list()) {
        if (not decompressed.has_node(v) or v_neighbors != decompressed.neighbors(v)) return false;
    }

    return true;
}

void apply_reductions(Graph& graph, Cover& cover) {
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
        std::cerr << "Adding a backtracking clique: ";
        print_unordered_set(R);
        return;
    }

    while (not P.empty()) {
        node_t v = *P.cbegin();
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
    assert(graph.has_edge(v1, v2));

    size_t result = 0;
    node_container_t common_neighbors;
    compute_common_neighbors(graph, cover, {v1, v2}, common_neighbors);

    for (auto it1 = common_neighbors.cbegin(); it1 != common_neighbors.cend(); it1++) {
        if (cover.is_removed(*it1)) continue;
        for (auto it2 = std::next(it1, 1); it2 != common_neighbors.cend(); it2++) {
            if (cover.is_removed(*it2)) continue;
            result += not graph.has_edge(*it1, *it2);
        }
    }

    return result;
}

void compute_edge_clique_cover(Graph& graph, Cover& cover) {
    // Apply reductions. If it works, then we're done.
    apply_reductions(graph, cover);

    if (cover.removed_nodes.size() == graph.n or is_edge_clique_cover(graph, cover))
        return;
    
    // Pick the lowest score edge. (The edge that participates in the fewest maximal cliques.)
    size_t best_score = SIZE_MAX;
    std::pair<node_t, node_t> best_edge = {UINT32_MAX, UINT32_MAX}; // If you want to change node_t,
                                                                    // you'll need to change this as well (and NodePairHash)
    std::cerr << "Best edge: " << best_edge.first << " " << best_edge.second << std::endl;
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

    if (best_edge.first == UINT32_MAX) {
        std::cerr << "Best edge didn't get set. Probably all the nodes are removed." << std::endl;
        exit(1);
    }

    // Make R, P, and X.
    // R contains vertices that have to be in the clique (for us, v1 and v2)
    // P contains vertices that could be in the clique (for us, their common neighbors)
    // X contains the clique as it is built. It's initialized empty.
    std::vector<node_container_t> cliques;
    node_container_t common_neighbors;
    compute_common_neighbors(graph, cover, {best_edge.first, best_edge.second}, common_neighbors);
    node_container_t R = {best_edge.first, best_edge.second};
    node_container_t X;
    compute_all_maximal_cliques(graph, cover, R, common_neighbors, X, cliques);

    std::vector<Cover> new_covers;
    for (node_container_t const& clique : cliques) {
        new_covers.push_back(cover); // Makes a copy
        new_covers[new_covers.size() - 1].cover_clique(clique);
    }

    size_t min_clique_cover = SIZE_MAX;
    for (Cover& new_cover : new_covers) {
        compute_edge_clique_cover(graph, new_cover);
        if (new_cover.cliques.size() < min_clique_cover) {
            min_clique_cover = new_cover.cliques.size();
            cover = new_cover;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./ecc <graph_file>" << std::endl;
        exit(1);
    }

    std::string filename = std::string(argv[1]);

    Graph graph = Graph(filename);
    Cover cover = Cover();

    Timer t;
    t.start();

    compute_edge_clique_cover(graph, cover);

    double elapsed = t.stop();

    std::cerr << "Time elapsed: " << elapsed << " ms." << std::endl;

    cover.unshadow();

    bool correct = is_edge_clique_cover(Graph(filename), cover);
    std::cerr << "It takes ";
    std::cout << cover.cliques.size();
    std::cerr << " cliques to cover the edges of this graph." << std::endl;
    std::cerr << "This edge clique cover is " << std::string(correct ? "" : "not ") << "correct." << std::endl;

    return 0;
}