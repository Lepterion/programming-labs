#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>

struct Edge {
    int to;
    double weight;
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class IGraph {
protected:
    int num_vertices;
    bool directed;

public:
    IGraph(int v, bool dir) : num_vertices(v), directed(dir) {}
    virtual ~IGraph() = default;

    virtual void add_edge(int u, int v, double weight = 1.0) = 0;
    virtual bool has_edge(int u, int v) const = 0;
    virtual double get_weight(int u, int v) const = 0;

    virtual std::vector<Edge> get_neighbors(int u) const = 0;

    int get_vertex_count() const { return num_vertices; }
    bool is_directed() const { return directed; }

    virtual void print() const = 0;
};