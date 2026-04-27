#pragma once
#include "graph_base.h"
#include <vector>
#include <functional>

namespace GraphAlgorithms {
    bool is_connected(const IGraph& graph);
    std::vector<std::vector<int>> find_connected_components(const IGraph& graph);
    bool is_acyclic(const IGraph& graph);

    void dfs(const IGraph& graph, int start, bool sort_neighbors, std::vector<int>& visited_order);
    void bfs(const IGraph& graph, int start, bool sort_neighbors, std::vector<int>& visited_order);

    std::vector<double> dijkstra(const IGraph& graph, int start);
    std::vector<std::vector<double>> floyd_warshall(const IGraph& graph);
    std::vector<double> bellman_ford(const IGraph& graph, int start);
    std::vector<int> a_star(const IGraph& graph, int start, int goal, std::function<double(int)> heuristic);

    std::vector<int> kahn_topological_sort(const IGraph& graph);

    std::vector<Edge> spanning_tree_bfs(const IGraph& graph, double& total_weight);
    std::vector<Edge> spanning_tree_dfs(const IGraph& graph, double& total_weight);

    std::vector<Edge> mst_kruskal(const IGraph& graph, double& total_weight);
    std::vector<Edge> mst_prim(const IGraph& graph, double& total_weight);
}