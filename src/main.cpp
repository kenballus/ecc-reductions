#include <iostream>
#include <chrono>
#include <string>
#include <limits>

#include "ecc.hpp"
#include "cover.hpp"
#include "graph.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./ecc <graph_file>" << std::endl;
        exit(1);
    }

    std::string const filename = std::string(argv[1]);

    Graph const graph = Graph(filename);
    Cover cover = Cover();

    size_t k = std::numeric_limits<size_t>::max();

    auto const start = std::chrono::high_resolution_clock::now();
    compute_edge_clique_cover(graph, cover, k);
    auto const cover_found_time = std::chrono::high_resolution_clock::now();
    std::cerr << "Time to find cover: " << std::chrono::duration_cast<std::chrono::nanoseconds>(cover_found_time - start).count() << " ns." << "\n";

    bool const decompression_is_correct = decompress_verify(Graph(filename), cover);
    bool const all_edges_covered = is_edge_clique_cover(Graph(filename), cover);
    std::cerr << "Cover:" << "\n";
    for (auto const& clique : cover.cliques) {
        for (auto const& node : clique) {
            std::cerr << node << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "It takes ";
    std::cout << cover.cliques.size();
    std::cerr << " cliques to cover the edges of this graph." << "\n";
    std::cerr << "Every edge is " << std::string(all_edges_covered ? "" : "not ") << "included in this cover." << "\n";
    std::cerr << "Decompression " << std::string(decompression_is_correct ? "verifies" : "does not verify") << " that this is an edge clique cover." << "\n";

    return not (decompression_is_correct && all_edges_covered);
}
