#include "graph_representations.h"
#include <limits>

// --- AdjacencyMatrixGraph Implementation ---

AdjacencyMatrixGraph::AdjacencyMatrixGraph(int v, bool dir)
    : IGraph(v, dir),
      INF(std::numeric_limits<double>::infinity()),
      matrix(v, std::vector<double>(v, std::numeric_limits<double>::infinity())) {}

void AdjacencyMatrixGraph::add_edge(int u, int v, double weight) {
    matrix[u][v] = weight;
    if (!directed) matrix[v][u] = weight;
}

bool AdjacencyMatrixGraph::has_edge(int u, int v) const {
    return matrix[u][v] != INF;
}

double AdjacencyMatrixGraph::get_weight(int u, int v) const {
    return matrix[u][v];
}

std::vector<Edge> AdjacencyMatrixGraph::get_neighbors(int u) const {
    std::vector<Edge> neighbors;
    for (int v = 0; v < num_vertices; ++v) {
        if (matrix[u][v] != INF) {
            neighbors.push_back({v, matrix[u][v]});
        }
    }

    return neighbors;
}

void AdjacencyMatrixGraph::print() const {}


// --- AdjacencyListGraph Implementation ---

AdjacencyListGraph::AdjacencyListGraph(int v, bool dir)
    : IGraph(v, dir), adj_list(v) {}

void AdjacencyListGraph::add_edge(int u, int v, double weight) {
    adj_list[u].push_back({v, weight});
    if (!directed) adj_list[v].push_back({u, weight});
}

bool AdjacencyListGraph::has_edge(int u, int v) const {
    for (const auto& edge : adj_list[u]) {
        if (edge.to == v) return true;
    }

    return false;
}

double AdjacencyListGraph::get_weight(int u, int v) const {
    for (const auto& edge : adj_list[u]) {
        if (edge.to == v) return edge.weight;
    }

    return std::numeric_limits<double>::infinity();
}

std::vector<Edge> AdjacencyListGraph::get_neighbors(int u) const {
    return adj_list[u];
}

void AdjacencyListGraph::print() const {}


// --- BitVectorGraph Implementation ---

BitVectorGraph::BitVectorGraph(const int v, const bool dir)
    : IGraph(v, dir), bit_matrix(v, std::vector<bool>(v, false)) {}

long long BitVectorGraph::edge_hash(int u, int v) const {
    return (long long)u * num_vertices + v;
}

void BitVectorGraph::add_edge(const int u, const int v, const double weight) {
    bit_matrix[u][v] = true;
    weights[edge_hash(u, v)] = weight;
    if (!directed) {
        bit_matrix[v][u] = true;
        weights[edge_hash(v, u)] = weight;
    }
}

bool BitVectorGraph::has_edge(const int u, const int v) const {
    return bit_matrix[u][v];
}

double BitVectorGraph::get_weight(const int u, const int v) const {
    if (!bit_matrix[u][v]) return std::numeric_limits<double>::infinity();
    return weights.at(edge_hash(u, v));
}

std::vector<Edge> BitVectorGraph::get_neighbors(const int u) const {
    std::vector<Edge> neighbors;
    for (int v = 0; v < num_vertices; ++v) {
        if (bit_matrix[u][v]) {
            neighbors.push_back({v, weights.at(edge_hash(u, v))});
        }
    }

    return neighbors;
}

void BitVectorGraph::print() const {}