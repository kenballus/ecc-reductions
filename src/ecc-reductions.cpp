#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>

#include "ecc-reductions.hpp"
#include "cover.hpp"
#include "graph.hpp"
#include "timer.hpp"

bool apply_rule_one(Graph& graph, Cover& cover) {
    bool ret = false;

    std::vector<node_t> to_remove;
    for (auto const& [v1, v1_neighbors] : graph.adj_list) {
        bool all_edges_covered = true;
        for (node_t v2 : v1_neighbors) {
            if (!cover.is_covered(v1, v2)) {
                all_edges_covered = false;
                break;
            }
        }

        if (all_edges_covered) {
            graph.unlink_node(v1);
            to_remove.push_back(v1);
            ret = true;
        }
    }

    for (node_t v : to_remove) {
        graph.delete_node(v);
    }

    return ret;
}

bool apply_rule_two(Graph& graph, Cover& cover) {
    size_t ret = 0;

    for (auto const& [v1, v1_neighbors] : graph.adj_list) {
        for (node_t v2 : v1_neighbors) {
            if (v2 <= v1 or cover.is_covered(v1, v2)) continue;

            std::unordered_set<node_t> common_neighbors;
            graph.find_common_neighbors(v1, v2, common_neighbors);
            common_neighbors.insert(v1);
            common_neighbors.insert(v2);

            if (graph.is_clique(common_neighbors)) {
                cover.cover_clique(common_neighbors);
                ret++;
            }
        }
    }

    return ret;
}

bool apply_rule_three(Graph& graph, Cover& cover) {
    // Assumes full reduction wrt rules 1 and 2.
    bool ret = false;

    std::vector<node_t> to_delete;
    for (auto const& kv_pair : graph.adj_list) {
        node_t v = kv_pair.first;

        std::unordered_set<node_t> prisoners;
        std::unordered_set<node_t> exits;
        graph.find_prisoners_and_exits(v, prisoners, exits);

        bool deleting_v = false;
        if (!prisoners.empty() && graph.prisoners_dominate_exits(prisoners, exits)) {
            for (node_t prisoner : prisoners) {
                cover.shadow_node(prisoner, v);
            }
            deleting_v = true;
            ret = true;
        }
        if (deleting_v) {
            to_delete.push_back(v);
            graph.unlink_node(v);
        }
    }

    for (node_t v : to_delete) {
        graph.delete_node(v);
    }

    return ret;
}

void fully_reduce(Graph& graph, Cover& cover) {
    while (true) {
        if (apply_rule_one(graph, cover))
            continue;
        if (apply_rule_two(graph, cover))
            continue;
        if (apply_rule_three(graph, cover))
            continue;
        return;
    }
}

void brute_force(Graph& graph, Cover& cover) {
    // to do
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./ecc-reductions <graph_file>" << std::endl;
        exit(1);
    }
    Graph graph = Graph(argv[1]);
    Cover cover = Cover();

    std::cout << "Graph has " << graph.e << " edges and " << graph.n << " nodes." << std::endl;

    Timer t;
    t.start();
    fully_reduce(graph, cover);
    double elapsed = t.stop();

    std::cout << "Time elapsed: " << elapsed << " ms." << std::endl;
    std::cout << "Reductions found " << cover.size << " cliques." << std::endl;
    std::cout << "Reduced graph has " << graph.e << " edges and " << graph.n << " nodes." << std::endl;

    return 0;
}
