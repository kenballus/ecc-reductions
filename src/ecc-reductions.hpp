#include "graph.hpp"
#include "cover.hpp"

bool apply_rule_one(Graph&, Cover&);
bool apply_rule_two(Graph&, Cover&);
bool apply_rule_three(Graph&, Cover&);
void fully_reduce(Graph&, Cover&, std::vector<size_t> const&);