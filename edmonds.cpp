#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int src, dst, weight;
};

// Graph class to handle the graph operations
class Graph {
public:
    Graph(int n) : n(n), edges() {}

    Graph clone() const {
        Graph g(n);
        g.edges = edges;
        return g;
    }

    // Method to add an edge to the graph
    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }

    // Method to find the minimum arborescence starting from a given root
    int findArborescence(int root) {
        vector<Edge> minimumEdges(n, {-1, -1, __INT_MAX__});
        vector<int> component(n, -1);

        // Find the minimum incoming edge for each node
        for (const auto& e : edges) {
            if (minimumEdges[e.dst].weight > e.weight) { // If the current edge is lighter than the minimum incoming edge
                minimumEdges[e.dst] = e; // Update the minimum incoming edge
            }
        }

        // 

        minimumEdges[root] = {root, root, 0}; // Root has no incoming edge

        int totalWeight = 0; // Total weight of the arborescence
        for (int i = 0; i < n; ++i) {
            if (minimumEdges[i].src < 0) return -1; // No arborescence exists
            totalWeight += minimumEdges[i].weight; // Add the weight of the minimum incoming edge
        }

        vector<int> cycleMarker(n, -1);
        int componentCount = 0;

        // Detect and contract cycles
        for (int i = 0; i < n; ++i) {
            if (cycleMarker[i] != -1) continue;

            int current = i;
            while (cycleMarker[current] == -1) {
                cycleMarker[current] = i; // Mark the current node
                current = minimumEdges[current].src; // Move to the parent node
            }

            if (cycleMarker[current] != i || current == root) continue;

            for (int v = minimumEdges[current].src; current != v; v = minimumEdges[v].src) {
                component[v] = componentCount; 
            }
            component[current] = componentCount++;
        }

        // If no cycles, return the total weight
        if (componentCount == 0) return totalWeight;

        // Contract the graph
        for (int i = 0; i < n; ++i) {
            if (component[i] == -1) component[i] = componentCount++;
        }

        vector<Edge> contractedEdges;
        for (const auto& e : edges) {
            // If the edge is not part of a cycle
            if (component[e.src] != component[e.dst] && component[e.dst] != component[root]) {
                // Add the edge to the contracted graph
                contractedEdges.push_back({component[e.src], component[e.dst], e.weight - minimumEdges[e.dst].weight});
            }
        }

        // Recursively find arborescence in the contracted graph
        edges.swap(contractedEdges);
        n = componentCount;
        root = component[root];

        // Recursively find arborescence in the contracted graph
        return findArborescence(root) + totalWeight;
    }

private:
    int n;
    vector<Edge> edges;
};

// Function to measure the current time
double currentTime() {
    static clock_t lastTime;
    clock_t currentTime = clock();
    double elapsed = double(currentTime - lastTime) / CLOCKS_PER_SEC;
    lastTime = currentTime;
    return elapsed;
}

int main() {
    for (int q = 0; q <= 1000; ++q) {
        srand(q);

        int nodeCount = 1000;
        Graph graph(nodeCount);

        // Adding random edges to the graph
        for (int i = 0; i < nodeCount; ++i) {
            for (int k = 0; k < 10; ++k) {
                int j;
                do {
                    j = rand() % nodeCount;
                } while (i == j);
                int w = 1 + rand() % 100;
                graph.addEdge(i, j, w);
            }
        }

        currentTime();
        
    
        double result = graph.findArborescence(0);
        cout << result << " " << currentTime() << endl;
    }
}
