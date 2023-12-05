#include <vector>

# pragma once

struct Edge {
public:
  int src, dst;
  double weight;
  Edge() {}
  Edge(int src, int dst, double weight) : src(src), dst(dst), weight(weight) {}
};

struct Graph {
    int n;
    std::vector<Edge> edges;
    Graph(int n) : n(n), edges() {}
    
    void addEdge(int src, int dst, double weight) {
        edges.push_back({src, dst, weight});
    }
};

class MSA {
    public:
        MSA(Graph graph) : graph(graph) {};
        Graph graph;
        virtual double findArborescence(int root) = 0;
};

# pragma once