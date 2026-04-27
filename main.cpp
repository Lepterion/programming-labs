#include <iostream>
#include <chrono>
#include "graph_utils.h"
#include "graph_algorithms.h"

using namespace std;
using namespace std::chrono;

/*
 * Обрані завдання:
 * Блок 0: 1, 2, 4
 * Блок 1: 5, 6, 7
 * Блок 2: 11, 13
 * Блок 3: 14, 15, 16, A*
 * Блок 4: 17
 * Блок 5: 19, 20
 * Блок 6: 21, 22
 */

void run_demo_mode() {
    cout << "\n=== DEMONSTRATION MODE ===\n";
    auto graph = GraphUtils::create_graph(GraphRepresentation::List, 5, false);
    graph->add_edge(0, 1, 10);
    graph->add_edge(0, 2, 5);
    graph->add_edge(1, 2, 2);
    graph->add_edge(1, 3, 1);
    graph->add_edge(2, 3, 9);
    graph->add_edge(3, 4, 4);

    cout << "[Block 0] Initial graph (Adjacency Lists):\n";
    GraphUtils::print_graph(*graph);

    cout << "\n[Block 0] Conversion to Bit Vector:\n";
    auto bit_graph = GraphUtils::convert_graph(*graph, GraphRepresentation::BitVector);
    GraphUtils::print_graph(*bit_graph);

    cout << "\n[Block 1] Acyclic: " << (GraphAlgorithms::is_acyclic(*graph) ? "Yes" : "No (contains cycles)") << "\n";
    cout << "[Block 1] Connected: " << (GraphAlgorithms::is_connected(*graph) ? "Yes" : "No") << "\n";

    cout << "\n[Block 2] BFS traversal (from vertex 0):\n";
    vector<int> bfs_order;
    GraphAlgorithms::bfs(*graph, 0, false, bfs_order);
    for (int v : bfs_order) cout << v << " ";
    cout << "\n";

    cout << "\n[Block 3] Dijkstra from vertex 0:\n";
    auto dists = GraphAlgorithms::dijkstra(*graph, 0);
    for (int i = 0; i < dists.size(); ++i) cout << "To " << i << " = " << dists[i] << "\n";

    double mst_weight = 0;
    auto mst = GraphAlgorithms::mst_kruskal(*graph, mst_weight);
    cout << "\n[Block 6] Minimum Spanning Tree (Kruskal). Total weight: " << mst_weight << "\nEdges: ";
    for (const auto& e : mst) cout << "[-" << e.to << " (w:" << e.weight << ")] ";
    cout << "\n=============================================\n";
}

void run_benchmark_mode() {
    cout << "\n=== AUTOMATIC MODE (BENCHMARK) ===\n";
    int vertices = 500; // Трохи зменшено для швидшого проходження в Debug
    int edges = 2000;

    cout << "[1/5] Generating Adjacency Matrix (" << vertices << " vertices, " << edges << " edges)..." << endl;
    auto matrix_graph = GraphUtils::generate_random_graph(GraphRepresentation::Matrix, vertices, edges, false);

    cout << "[2/5] Generating Adjacency List..." << endl;
    auto list_graph = GraphUtils::generate_random_graph(GraphRepresentation::List, vertices, edges, false);

    double w;

    cout << "[3/5] Running Prim's Algorithm on Matrix..." << endl;
    auto start = high_resolution_clock::now();
    GraphAlgorithms::mst_prim(*matrix_graph, w);
    auto stop = high_resolution_clock::now();
    cout << "      Done in " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    cout << "[4/5] Running Prim's Algorithm on List..." << endl;
    start = high_resolution_clock::now();
    GraphAlgorithms::mst_prim(*list_graph, w);
    stop = high_resolution_clock::now();
    cout << "      Done in " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    cout << "[5/5] Running Dijkstra's Algorithm on List..." << endl;
    start = high_resolution_clock::now();
    GraphAlgorithms::dijkstra(*list_graph, 0);
    stop = high_resolution_clock::now();
    cout << "      Done in " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    cout << "Benchmark complete!\n";
}

void run_interactive_mode() {
    cout << "\n=== INTERACTIVE MODE ===\n";
    int v, e;
    cout << "Enter number of vertices: "; cin >> v;
    cout << "Enter number of edges: "; cin >> e;

    auto graph = GraphUtils::create_graph(GraphRepresentation::List, v, false);
    cout << "Enter edges in format (from to weight):\n";
    cout << "[Vertices must be between 0 and " << v - 1 << ". Please provide ALL " << e * 3 << " numbers.]\n";

    int edges_added = 0;
    while (edges_added < e) {
        int from, to; double weight;
        cin >> from >> to >> weight;

        if (from < 0 || from >= v || to < 0 || to >= v) {
            cout << "Error: Vertex out of bounds. Please enter values between 0 and " << v - 1 << ".\n";
            continue;
        }

        graph->add_edge(from, to, weight);
        edges_added++;
    }

    cout << "\nGraph successfully created!\n";
    GraphUtils::print_graph(*graph);

    double total_w = 0;
    auto mst = GraphAlgorithms::mst_prim(*graph, total_w);
    cout << "Weight of Minimum Spanning Tree (Prim): " << total_w << "\n";
}

int main() {
    int choice = 0;
    while (choice != 4) {
        cout << "\nLab 5. Main Menu:\n";
        cout << "1. Demonstration mode\n";
        cout << "2. Benchmark mode\n";
        cout << "3. Interactive mode\n";
        cout << "4. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: run_demo_mode(); break;
            case 2: run_benchmark_mode(); break;
            case 3: run_interactive_mode(); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    }
    return 0;
}