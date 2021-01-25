#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>

#include "ecc-reductions.hpp"
#include "cover.hpp"
#include "graph.hpp"
#include "timer.hpp"

void apply_rule_one(Graph& graph, Cover& cover) {
    for (node_t v1 = 0; v1 < graph.n; v1++) {
        if (!graph.exists(v1)) continue;

        bool all_edges_covered = true;
        for (node_t v2 : graph.neighbors(v1)) {
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
            if (v2 <= v1 or cover.is_covered(v1, v2)) continue;

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

void apply_rule_three_prime(Graph& graph, Cover& cover) {
    for (node_t v1 = 0; v1 < graph.n; v1++) {
        if (!graph.exists(v1)) continue;

        std::vector<std::pair<node_t, node_t>> to_shadow;
        for (node_t v2 : graph.neighbors(v1)) {
            if (v2 <= v1) continue;

            if (graph.has_same_neighbors(v1, v2)) {
                to_shadow.push_back(std::make_pair(v1, v2));
            }
        }

        for (auto it = to_shadow.begin(); it != to_shadow.end(); it++) {
            graph.remove_edge((*it).first, (*it).second);
        }
    }
}

void apply_rule_three(Graph& graph, Cover& cover) {

}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./ecc-reductions <graph_file>" << std::endl;
        exit(1);
    }
    Graph graph = Graph(argv[1]);
    Cover cover = Cover();

    std::cout << graph << std::endl;

    Timer t;
    t.start();

    apply_rule_two(graph, cover);
    apply_rule_one(graph, cover);
    apply_rule_three_prime(graph, cover);

    std::cout << "Time elapsed: " << t.stop() << " ms." << std::endl;

    std::cout << graph << std::endl;

    std::cout << "Reductions found " << cover.size << " cliques." << std::endl;

    return 0;
}
