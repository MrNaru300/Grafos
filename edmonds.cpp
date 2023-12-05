/*
    Code based on: Spaghetti Source Code - Edmonds' Algorithm
    https://github.com/spaghetti-source/algorithm/blob/master/graph/arborescence2.cc

    Minimum Arborescence (Chu-Liu/Edmonds)

    Description:
      Let G = (V, E) be a weighted directed graph.
      For a vertex r, an edge-set T is called r-arborescense if
        (1) T is a spanning tree (with forgetting directions),
        (2) for each u in V, indeg_T(u) <= 1, indeg_T(r) = 0.
      The program finds the minimum weight of r-arborescence.


    Algorithm:
      Chu-Liu/Edmonds' recursive shrinking.
      At first, it finds a minimum incomming edge for each v in V.
      Then, if it forms a arborescence, it is a solution,
      and otherwise, it contracts a cycle and iterates the procedure.


    Complexity: 
      O(mn)


    Remark:
      More efficient (but long) version is also available.


    References: 
      Y. J. Chu and T. H. Liu (1965): 
      On the shortest arborescence of a directed graph,
      Science Sinica, vol. 14, pp. 1396--1400.

      J. Edmonds (1967): 
      Optimum branchings
      Journal on Research of the National Bureau of Standards, 71B,
      pp. 233--240.

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
