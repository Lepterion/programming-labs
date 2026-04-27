#include "graph_algorithms.h"
#include <queue>
#include <algorithm>
#include <limits>
#include <set>
#include <numeric>

namespace GraphAlgorithms {

    const double INF = std::numeric_limits<double>::infinity();

    std::vector<Edge> get_processed_neighbors(const IGraph& graph, int u, bool sort_by_weight) {
        auto neighbors = graph.get_neighbors(u);
        if (sort_by_weight) {
            std::sort(neighbors.begin(), neighbors.end());
        }

        return neighbors;
    }

    void dfs_simple(const IGraph& graph, int u, std::vector<bool>& visited) {
        visited[u] = true;
        for (const auto& edge : graph.get_neighbors(u)) {
            if (!visited[edge.to]) dfs_simple(graph, edge.to, visited);
        }
    }

    bool is_connected(const IGraph& graph) {
        int V = graph.get_vertex_count();
        if (V == 0) return true;
        std::vector<bool> visited(V, false);
        dfs_simple(graph, 0, visited);
        for (bool v : visited) if (!v) return false;
        return true;
    }

    std::vector<std::vector<int>> find_connected_components(const IGraph& graph) {
        int V = graph.get_vertex_count();
        std::vector<bool> visited(V, false);
        std::vector<std::vector<int>> components;

        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                std::vector<int> component;
                auto local_dfs = [&](auto& self, int u) -> void {
                    visited[u] = true;
                    component.push_back(u);
                    for (const auto& edge : graph.get_neighbors(u)) {
                        if (!visited[edge.to]) self(self, edge.to);
                    }
                };
                local_dfs(local_dfs, i);
                components.push_back(component);
            }
        }

        return components;
    }

    bool is_acyclic_util(const IGraph& graph, int u, std::vector<int>& state, int parent) {
        state[u] = 1;
        for (const auto& edge : graph.get_neighbors(u)) {
            if (state[edge.to] == 0) {
                if (!is_acyclic_util(graph, edge.to, state, u)) return false;
            } else if (state[edge.to] == 1) {
                if (!graph.is_directed() && edge.to == parent) continue;
                return false;
            }
        }

        state[u] = 2;
        return true;
    }

    bool is_acyclic(const IGraph& graph) {
        int V = graph.get_vertex_count();
        std::vector<int> state(V, 0);
        for (int i = 0; i < V; ++i) {
            if (state[i] == 0) {
                if (!is_acyclic_util(graph, i, state, -1)) return false;
            }
        }

        return true;
    }

    void dfs_util(const IGraph& graph, int u, std::vector<bool>& visited, bool sort_neighbors, std::vector<int>& order) {
        visited[u] = true;
        order.push_back(u);

        for (const auto& edge : get_processed_neighbors(graph, u, sort_neighbors)) {
            if (!visited[edge.to]) {
                dfs_util(graph, edge.to, visited, sort_neighbors, order);
            }
        }
    }

    void dfs(const IGraph& graph, int start, bool sort_neighbors, std::vector<int>& visited_order) {
        std::vector<bool> visited(graph.get_vertex_count(), false);
        dfs_util(graph, start, visited, sort_neighbors, visited_order);
    }

    void bfs(const IGraph& graph, int start, bool sort_neighbors, std::vector<int>& visited_order) {
        std::vector<bool> visited(graph.get_vertex_count(), false);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            visited_order.push_back(u);

            for (const auto& edge : get_processed_neighbors(graph, u, sort_neighbors)) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
    }

    std::vector<double> dijkstra(const IGraph& graph, int start) {
        int V = graph.get_vertex_count();
        std::vector<double> dist(V, INF);
        using P = std::pair<double, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : graph.get_neighbors(u)) {
                if (dist[u] + edge.weight < dist[edge.to]) {
                    dist[edge.to] = dist[u] + edge.weight;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        return dist;
    }

    std::vector<std::vector<double>> floyd_warshall(const IGraph& graph) {
        int V = graph.get_vertex_count();
        std::vector<std::vector<double>> dist(V, std::vector<double>(V, INF));

        for (int i = 0; i < V; ++i) dist[i][i] = 0;
        for (int u = 0; u < V; ++u) {
            for (const auto& edge : graph.get_neighbors(u)) {
                dist[u][edge.to] = edge.weight;
            }
        }

        for (int k = 0; k < V; ++k) {
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF) {
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        return dist;
    }

    std::vector<double> bellman_ford(const IGraph& graph, int start) {
        int V = graph.get_vertex_count();
        std::vector<double> dist(V, INF);
        dist[start] = 0;

        struct FullEdge { int u, v; double w; };
        std::vector<FullEdge> edges;
        for (int u = 0; u < V; ++u) {
            for (const auto& edge : graph.get_neighbors(u)) {
                edges.push_back({u, edge.to, edge.weight});
            }
        }

        for (int i = 0; i < V - 1; ++i) {
            for (const auto& e : edges) {
                if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                }
            }
        }

        return dist;
    }

    std::vector<int> a_star(const IGraph& graph, int start, int goal, std::function<double(int)> heuristic) {
        int V = graph.get_vertex_count();
        std::vector<double> g_score(V, INF);
        std::vector<int> came_from(V, -1);

        using P = std::pair<double, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> open_set;

        g_score[start] = 0;
        open_set.push({heuristic(start), start});

        while (!open_set.empty()) {
            int u = open_set.top().second;
            open_set.pop();

            if (u == goal) {
                std::vector<int> path;
                for (int curr = goal; curr != -1; curr = came_from[curr]) {
                    path.push_back(curr);
                }
                std::ranges::reverse(path);
                return path;
            }

            for (const auto& edge : graph.get_neighbors(u)) {
                double tentative_g = g_score[u] + edge.weight;
                if (tentative_g < g_score[edge.to]) {
                    came_from[edge.to] = u;
                    g_score[edge.to] = tentative_g;
                    double f_score = tentative_g + heuristic(edge.to);
                    open_set.push({f_score, edge.to});
                }
            }
        }

        return {};
    }

    std::vector<int> kahn_topological_sort(const IGraph& graph) {
        int V = graph.get_vertex_count();
        std::vector<int> in_degree(V, 0);

        for (int u = 0; u < V; ++u) {
            for (const auto& edge : graph.get_neighbors(u)) {
                in_degree[edge.to]++;
            }
        }

        std::queue<int> q;
        for (int i = 0; i < V; i++) {
            if (in_degree[i] == 0) q.push(i);
        }

        std::vector<int> top_order;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            top_order.push_back(u);

            for (const auto& edge : graph.get_neighbors(u)) {
                if (--in_degree[edge.to] == 0) {
                    q.push(edge.to);
                }
            }
        }

        if (top_order.size() != V) return {};
        return top_order;
    }

    std::vector<Edge> spanning_tree_bfs(const IGraph& graph, double& total_weight) {
        total_weight = 0;
        int V = graph.get_vertex_count();
        if (V == 0) return {};

        std::vector<Edge> tree_edges;
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        visited[0] = true;
        q.push(0);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& edge : graph.get_neighbors(u)) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    tree_edges.push_back({edge.to, edge.weight});
                    total_weight += edge.weight;
                    q.push(edge.to);
                }
            }
        }

        return tree_edges;
    }

    void spanning_tree_dfs_util(const IGraph& graph, const int u, std::vector<bool>& visited, std::vector<Edge>& tree_edges, double& total_weight) {
        visited[u] = true;
        for (const auto& edge : graph.get_neighbors(u)) {
            if (!visited[edge.to]) {
                tree_edges.push_back({edge.to, edge.weight});
                total_weight += edge.weight;
                spanning_tree_dfs_util(graph, edge.to, visited, tree_edges, total_weight);
            }
        }
    }

    std::vector<Edge> spanning_tree_dfs(const IGraph& graph, double& total_weight) {
        total_weight = 0;
        int V = graph.get_vertex_count();
        if (V == 0) return {};

        std::vector<Edge> tree_edges;
        std::vector<bool> visited(V, false);

        spanning_tree_dfs_util(graph, 0, visited, tree_edges, total_weight);

        return tree_edges;
    }

    class DSU {
        std::vector<int> parent, rank;
    public:
        DSU(int n) {
            parent.resize(n);
            rank.resize(n, 0);
            std::iota(parent.begin(), parent.end(), 0);
        }
        int find(int i) {
            if (parent[i] == i) return i;
            return parent[i] = find(parent[i]);
        }
        bool unite(int i, int j) {
            int root_i = find(i);
            int root_j = find(j);
            if (root_i != root_j) {
                if (rank[root_i] < rank[root_j]) parent[root_i] = root_j;
                else if (rank[root_i] > rank[root_j]) parent[root_j] = root_i;
                else {
                    parent[root_j] = root_i;
                    rank[root_i]++;
                }

                return true;
            }

            return false;
        }
    };

    std::vector<Edge> mst_kruskal(const IGraph& graph, double& total_weight) {
        total_weight = 0;
        int V = graph.get_vertex_count();
        std::vector<Edge> mst;

        struct FullEdge { int u, v; double w;
            bool operator<(const FullEdge& other) const { return w < other.w; }
        };

        std::vector<FullEdge> all_edges;
        for (int u = 0; u < V; ++u) {
            for (const auto& edge : graph.get_neighbors(u)) {
                if (graph.is_directed() || u < edge.to) {
                    all_edges.push_back({u, edge.to, edge.weight});
                }
            }
        }

        std::sort(all_edges.begin(), all_edges.end());
        DSU dsu(V);

        for (const auto& e : all_edges) {
            if (dsu.unite(e.u, e.v)) {
                mst.push_back({e.v, e.w});
                total_weight += e.w;
                if (mst.size() == V - 1) break;
            }
        }

        return mst;
    }

    std::vector<Edge> mst_prim(const IGraph& graph, double& total_weight) {
        total_weight = 0;
        int V = graph.get_vertex_count();
        std::vector<Edge> mst_edges;
        std::vector<bool> in_mst(V, false);

        using P = std::pair<double, std::pair<int, int>>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

        pq.push({0.0, {0, 0}});

        while (!pq.empty() && mst_edges.size() < V) {
            auto [weight, vertices] = pq.top();
            auto [u, v] = vertices;
            pq.pop();

            if (in_mst[v]) continue;

            in_mst[v] = true;
            total_weight += weight;
            if (u != v) mst_edges.push_back({v, weight});

            for (const auto& edge : graph.get_neighbors(v)) {
                if (!in_mst[edge.to]) {
                    pq.push({edge.weight, {v, edge.to}});
                }
            }
        }

        return mst_edges;
    }
}