#include <unordered_set>
#include <vector>
#include <string>

struct Graph {
    std::vector<std::unordered_set<size_t>*> adj_list;
    size_t n;
    size_t e;

    Graph(size_t);
    Graph(std::string);
    ~Graph();
    void add_edge(size_t, size_t);
    void remove_edge(size_t, size_t);

    bool has_edge(size_t, size_t) const;
};