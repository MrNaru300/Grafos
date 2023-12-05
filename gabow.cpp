#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "graph.h"

using namespace std;


// Disjoint set data structure for union-find operations
struct UnionFind {
  vector<int> parent; 
  UnionFind(int n) : parent(n, -1) { };

  // Unite two sets; return false if they are already in the same set
  bool unite(int u, int v) { 
    if ((u = root(u)) == (v = root(v))) return false;
    if (parent[u] > parent[v]) swap(u, v);
    parent[u] += parent[v]; parent[v] = u;
    return true;
  }

  // Check if two elements are in the same set
  bool find(int u, int v) { return root(u) == root(v); }

  // Find the root of the set containing 'u'
  int root(int u) { return parent[u] < 0 ? u : parent[u] = root(parent[u]); }

  // Get the size of the set containing 'u'
  int size(int u) { return -parent[root(u)]; }
};

// Skew heap data structure for mergeable heap operations
struct SkewHeap {
  struct Node {
    Node *child[2];
    Edge key;
    double delta;
  } *root;

  SkewHeap() : root(0) { }

  // Propagate the lazy updates in the heap
  void propagate(Node *node) {
    node->key.weight += node->delta;
    if (node->child[0]) node->child[0]->delta += node->delta;
    if (node->child[1]) node->child[1]->delta += node->delta;
    node->delta = 0;
  }

  // Merge two heaps and return the new root
  Node *merge(Node *a, Node *b) {
    if (!a || !b) return a ? a : b;
    propagate(a); propagate(b);
    if (a->key.weight > b->key.weight) swap(a, b);
    a->child[1] = merge(b, a->child[1]);
    swap(a->child[0], a->child[1]);
    return a;
  }

  // Insert a new element into the heap
  void push(Edge key) {
    Node *n = new Node();
    n->child[0] = n->child[1] = 0;
    n->key = key; n->delta = 0;
    root = merge(root, n);
  }

  // Remove the smallest element from the heap
  void pop() {
    propagate(root);
    Node *temp = root;
    root = merge(root->child[0], root->child[1]);
  }

  // Get the smallest element in the heap
  Edge top() {
    propagate(root);
    return root->key;
  }

  // Check if the heap is isEmpty
  bool isEmpty() { 
    return !root;
  }

  // Apply a delta to all elements in the heap
  void add(double delta) {
    root->delta += delta;
  }

  // Merge another skew heap into this heap
  void merge(SkewHeap x) {
    root = merge(root, x.root);
  }
};

// Class for solving the minimum arborescence problem using Gabow's algorithm
struct MSAGabow : public MSA {
  

  MSAGabow(Graph graph) : MSA(graph) { };

  // Solve the minimum arborescence problem for a given root
  int findArborescence(int root) {
    int n = graph.n;
    vector<Edge>& edges = graph.edges;

    UnionFind unionFind(n);
    vector<SkewHeap> heap(n);

    // Add the incoming edges to the heaps
    for (auto e: edges) 
      heap[e.dst].push(e);

    double score = 0;
    vector<int> seen(n, -1); // Mark the nodes that have been seen
    seen[root] = root; // Mark the root as seen
    for (int node = 0; node < n; ++node) { // For each node node
      vector<int> path;
      for (int current = node; seen[current] < 0;) { // Find the cycle

        path.push_back(current); // Add the current node to the path
        seen[current] = node; // Mark the current node
        if (heap[current].isEmpty()) return -1; // Return -1 if there is no arborescence

        Edge minEdge = heap[current].top(); // Get the minimum incoming edge
        score += minEdge.weight; // Add the weight of the minimum incoming edge
        heap[current].add(-minEdge.weight); // Cancel the minimum incoming edge
        heap[current].pop(); // Remove the minimum incoming edge

        int v = unionFind.root(minEdge.src); // Find the root of the other node

        // If the other node is not in the cycle, continue
        if (seen[v] == node) {
          SkewHeap newHeap;
          int w;

          // Merge the heaps of the nodes in the cycle
          do {
            w = path.back();
            path.pop_back();
            newHeap.merge(heap[w]);
          } while (unionFind.unite(v, w));
          heap[unionFind.root(v)] = newHeap; // Add the new heap to the root of the cycle
          seen[unionFind.root(v)] = -1; // Mark as unseen
        }
        current = unionFind.root(v);
      }
    }
    return score;
  }
};