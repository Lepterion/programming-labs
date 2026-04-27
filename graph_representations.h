#pragma once
#include "graph_base.h"
#include <vector>
#include <unordered_map>

// --- Task 1: Adjacency Matrix ---
class AdjacencyMatrixGraph : public IGraph {
private:
    std::vector<std::vector<double>> matrix;
    const double INF;

public:
    AdjacencyMatrixGraph(int v, bool dir);

    void add_edge(int u, int v, double weight = 1.0) override;
    bool has_edge(int u, int v) const override;
    double get_weight(int u, int v) const override;
    std::vector<Edge> get_neighbors(int u) const override;
    void print() const override;
};

// --- Task 2: Adjacency List ---
class AdjacencyListGraph : public IGraph {
private:
    std::vector<std::vector<Edge>> adj_list;

public:
    AdjacencyListGraph(int v, bool dir);

    void add_edge(int u, int v, double weight = 1.0) override;
    bool has_edge(int u, int v) const override;
    double get_weight(int u, int v) const override;
    std::vector<Edge> get_neighbors(int u) const override;
    void print() const override;
};

// --- Task 4: Bit Vector ---
class BitVectorGraph : public IGraph {
private:
    std::vector<std::vector<bool>> bit_matrix;
    std::unordered_map<long long, double> weights;

    long long edge_hash(int u, int v) const;

public:
    BitVectorGraph(int v, bool dir);

    void add_edge(int u, int v, double weight = 1.0) override;
    bool has_edge(int u, int v) const override;
    double get_weight(int u, int v) const override;
    std::vector<Edge> get_neighbors(int u) const override;
    void print() const override;
};