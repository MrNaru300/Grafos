#include <vector>

# pragma once

#define INF __INT_MAX__
struct Edge {
public:
  int src, dst, weight;
  Edge() {}
  Edge(int src, int dst, int weight) : src(src), dst(dst), weight(weight) {}
};

struct Graph {
    int n, totalWeight = 0;
    std::vector<Edge> edges;
    Graph(int n) : n(n), edges() {}
    
    void addEdge(int src, int dst, int weight) {
        edges.push_back({src, dst, weight});
        totalWeight += weight;
    }

    void removeEdge(int src, int dst) {
        for (int i = 0; i < edges.size(); ++i) {
            if (edges[i].src == src && edges[i].dst == dst) {
                edges.erase(edges.begin() + i);
                return;
            }
        }
    }

};

class MSA {
    public:
        MSA(Graph graph) : graph(graph) {};
        Graph graph;
        virtual int findArborescence(int root) = 0;
};