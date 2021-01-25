#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility> // for pair

#include "ecc-reductions.hpp"
#include "cover.hpp"
#include "graph.hpp"

void apply_rule_one(Graph& graph, Cover& cover) {
    for (node_t v1 = 0; v1 < graph.n; ++v1) {
        bool all_edges_covered = true;
        for (node_t v2 : graph.adj_list[v1]) {
            if (!cover.is_covered(v1, v2)) {
                all_edges_covered = false;
                break;
            }
        }

        if (all_edges_covered) {
            graph.remove_all_adjacent_edges(v1);
        }
    }
}

void apply_rule_two(Graph& graph, Cover& cover) {
    for (node_t v = 0; v < graph.n; ++v) {
        if (graph.adj_list[v].size() == 1) {
            cover.size++;
            std::cout << "Node " << v << " has one neighbor." << std::endl;
            graph.remove_edge(v, *(graph.neighbors(v).begin()));
        }
    }
}

int main() {
    Graph graph = Graph("test_graph.el");
    Cover cover = Cover();

    apply_rule_one(graph, cover);

    std::cout << graph << std::endl;
    std::cout << "Reductions found " << cover.size << " cliques." << std::endl;

    return 0;
}
