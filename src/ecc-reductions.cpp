#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>

#include "ecc-reductions.hpp"
#include "cover.hpp"
#include "graph.hpp"

void apply_rule_one(Graph& graph, Cover& cover) {
    for (node_t v1 = 0; v1 < graph.n; v1++) {
        if (!graph.exists(v1)) continue;

        bool all_edges_covered = true;
        for (node_t v2 : graph.adj_list[v1]) {
            if (!graph.exists(v2)) continue;

            if (!cover.is_covered(v1, v2)) {
                all_edges_covered = false;
                break;
            }
        }

        // A node is considered "removed" when it has no neighbors.
        if (all_edges_covered) {
            graph.remove_all_adjacent_edges(v1);
        }
    }
}

void apply_rule_two(Graph& graph, Cover& cover) {
    for (node_t v1 = 0; v1 < graph.n; v1++) {
        if (!graph.exists(v1)) continue;

        for (node_t v2 : graph.neighbors(v1)) {
            if (!graph.exists(v2) or v2 <= v1 or cover.is_covered(v1, v2)) continue;

            std::unordered_set<node_t> common_neighbors;
            graph.find_common_neighbors(v1, v2, common_neighbors);
            common_neighbors.insert(v1);
            common_neighbors.insert(v2);

            if (graph.is_clique(common_neighbors)) {
                cover.cover_clique(common_neighbors);
            }
        }
    }
}

int main() {
    Graph graph = Graph("test_graph.el");
    Cover cover = Cover();

    apply_rule_one(graph, cover);
    apply_rule_two(graph, cover);

    std::cout << graph << std::endl;
    std::cout << "Reductions found " << cover.size << " cliques." << std::endl;

    return 0;
}
