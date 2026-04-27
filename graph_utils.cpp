#include "graph_utils.h"
#include <iostream>
#include <iomanip>

namespace GraphUtils {

    std::unique_ptr<IGraph> create_graph(const GraphRepresentation type, int vertices, bool directed) {
        switch (type) {
            case GraphRepresentation::Matrix: return std::make_unique<AdjacencyMatrixGraph>(vertices, directed);
            case GraphRepresentation::List: return std::make_unique<AdjacencyListGraph>(vertices, directed);
            case GraphRepresentation::BitVector: return std::make_unique<BitVectorGraph>(vertices, directed);
            default: throw std::invalid_argument("Unknown graph representation");
        }
    }

    std::unique_ptr<IGraph> generate_random_graph(const GraphRepresentation type, const int vertices, int edges, const bool directed) {
        auto graph = create_graph(type, vertices, directed);

        std::random_device rd;
        std::mt19937 rng(rd());

        std::uniform_int_distribution<int> dist_v(0, vertices - 1);
        std::uniform_real_distribution<double> dist_w(1.0, 100.0);

        int edges_added = 0;
        int max_possible_edges = directed ? (vertices * (vertices - 1)) : (vertices * (vertices - 1)) / 2;
        if (edges > max_possible_edges) edges = max_possible_edges;

        int attempts = 0;
        while (edges_added < edges && attempts < edges * 10) {
            int u = dist_v(rng);
            int v = dist_v(rng);
            if (u != v && !graph->has_edge(u, v)) {
                graph->add_edge(u, v, dist_w(rng));
                edges_added++;
                attempts = 0;
            } else {
                attempts++;
            }
        }

        return graph;
    }

    std::unique_ptr<IGraph> convert_graph(const IGraph& source, const GraphRepresentation target_type) {
        int V = source.get_vertex_count();
        bool dir = source.is_directed();
        auto target = create_graph(target_type, V, dir);

        for (int u = 0; u < V; ++u) {
            for (const auto& edge : source.get_neighbors(u)) {
                if (dir || u <= edge.to) {
                    target->add_edge(u, edge.to, edge.weight);
                }
            }
        }

        return target;
    }

    void print_graph(const IGraph& graph) {
        int V = graph.get_vertex_count();
        std::cout << "Graph (" << V << " vertices, " << (graph.is_directed() ? "directed" : "undirected") << "):\n";
        for (int u = 0; u < V; ++u) {
            std::cout << u << " -> ";
            auto neighbors = graph.get_neighbors(u);
            if (neighbors.empty()) {
                std::cout << "no neighbors";
            } else {
                for (const auto& edge : neighbors) {
                    std::cout << "[" << edge.to << " (w:" << std::fixed << std::setprecision(1) << edge.weight << ")] ";
                }
            }
            std::cout << "\n";
        }

        std::cout << "------------------------------------------\n";
    }
}