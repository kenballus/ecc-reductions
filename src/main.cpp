#include <iostream>
#include <chrono>
#include <string>
#include <limits>

#include "ecc.hpp"
#include "cover.hpp"
#include "graph.hpp"

// NOTE: output on stdout looks like this:
// ecc total_calls time

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: ./ecc [inital_k] < <graph_file>\n";
        exit(1);
    }

    Graph graph = Graph(std::cin);
    Cover cover = Cover(graph.n);

    size_t const k = argc > 1 ? std::stoi(argv[1]) : std::numeric_limits<size_t>::max();

    size_t total_calls = 0;
    auto const start = std::chrono::high_resolution_clock::now();
    // order_neighbors_by_neighbor_degree(graph);

    bool found_cover = compute_edge_clique_cover(graph, cover, k, total_calls);
    auto const cover_found_time = std::chrono::high_resolution_clock::now();

    std::cerr << "Cover:" << "\n";
    for (auto const& clique : cover.cliques) {
        for (auto const& node : clique) {
            std::cerr << node << " ";
        }
        std::cerr << "\n";
    }

    if (not found_cover) {
        if (k == std::numeric_limits<size_t>::max()) {
            std::cerr << "A cover cannot be computed, for some reason...\n";
            exit(1);
        }
        else {
            std::cerr << "Initial bound of";
            std::cout << " " << k;
            std::cerr << " is optimal.\n";
        }
    }
    else {
        std::cerr << "ECC number: ";
        std::cout << " " << cover.cliques.size();
        std::cerr << "\n";
    }

    std::cerr << "Total recursive calls:";
    std::cout << " " << total_calls;
    std::cerr << "\n";

    std::cerr << "Time (ns) to find cover:";
    std::cout << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(cover_found_time - start).count();
    std::cerr << "\n";

    bool const all_edges_covered = is_edge_clique_cover(graph, cover);
    bool const decompression_is_correct = decompress_verify(graph, cover);
    std::cerr << "Every edge is " << std::string(all_edges_covered ? "" : "not ") << "included in this cover." << "\n";
    std::cerr << "Decompression " << std::string(decompression_is_correct ? "verifies" : "does not verify") << " that this is an edge clique cover.\n";

    return not (decompression_is_correct and all_edges_covered);
}
