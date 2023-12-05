/*
    Code based on: Spaghetti Source Code - Edmonds' Algorithm
    https://github.com/spaghetti-source/algorithm/blob/master/graph/arborescence2.cc

*/

#include "graph.h"
#include <vector>

using namespace std;


struct spaghetti_edmonds : public Graph {
  
  spaghetti_edmonds(Graph graph) : Graph(graph) { }
  
  int findArborescence(int root) {
    int N = this->n;
    vector<Edge>& edges = this->edges;

    int score = 0;

    while (true) {
      vector<Edge> commingEdge(N, {-1,-1,(int)INF});
      vector<int> component(N, -1);
      for (auto edge: edges) {
      // cheapest comming edges
        if (commingEdge[edge.dst].weight > edge.weight) {
          commingEdge[edge.dst] = edge;
          }
        }
      commingEdge[root] = {root, root, 0};

      for (int vertex = 0; vertex < N; ++vertex) { // no comming edge ==> no aborescense
        if (commingEdge[vertex].src < 0) return -1;
        score += commingEdge[vertex].weight;
      }
      vector<int> mark(N, -1); // contract cycles
      int index = 0;
      for (int vertex = 0; vertex < N; ++vertex) {
        if (mark[vertex] != -1) continue; // already marked
        int currentVertex = vertex;
        // find cycle
        while (mark[currentVertex] == -1) {
          mark[currentVertex] = vertex;
          currentVertex = commingEdge[currentVertex].src; // move to parent
        }
        // if not cycle or not in cycle
        if (mark[currentVertex] != vertex || currentVertex == root) continue;

        // contract cycle
        for (int v = commingEdge[currentVertex].src; currentVertex != v; v = commingEdge[v].src) component[v] = index;
        // mark cycle
        component[currentVertex] = index++;
      }
      if (index == 0) return score; // found arborescence
      for (int i = 0; i < N; ++i) {// contract
        if (component[i] == -1) component[i] = index++;
      }


      vector<Edge> next;
      
      for (auto &e: edges){ 

        // add edges that are not in the same component
        if (component[e.src] != component[e.dst] && component[e.dst] != component[root]) {
          next.push_back({component[e.src], component[e.dst], e.weight - commingEdge[e.dst].weight});
        }
      }

      
      edges.swap(next);
      N = index; root = component[root];
    }
  }
};
