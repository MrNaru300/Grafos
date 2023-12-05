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
    int n;
    std::vector<Edge> edges;
    Graph(int n) : n(n), edges() {}
    
    void addEdge(int src, int dst, int weight) {
        edges.push_back({src, dst, weight});
    }


};

class MSA {
    public:
        MSA(Graph graph) : graph(graph) {};
        Graph graph;
        virtual int findArborescence(int root) = 0;
};