#pragma once
#include "graph_representations.h"
#include <memory>
#include <random>

enum class GraphRepresentation {
    Matrix,
    List,
    BitVector
};

namespace GraphUtils {
    std::unique_ptr<IGraph> create_graph(GraphRepresentation type, int vertices, bool directed);
    std::unique_ptr<IGraph> generate_random_graph(GraphRepresentation type, int vertices, int edges, bool directed);
    std::unique_ptr<IGraph> convert_graph(const IGraph& source, GraphRepresentation target_type);
    void print_graph(const IGraph& graph);
}