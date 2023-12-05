#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

// Structure to represent an edge in the graph
struct Edge { 
  int src, dst; 
  double weight;
};

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

const double INF = 9999;

// Class for solving the minimum arborescence problem
struct MinimumArborescence {
  vector<Edge> edges;
  int n;

  MinimumArborescence(int n = 0) : n(n), edges() { }

  // Add an edge to the graph
  void addEdge(int src, int dst, double weight) {
    edges.push_back({src, dst, weight});
  }



  // Solve the minimum arborescence problem for a given root
  double solve(int root) {
    UnionFind unionFind(n);
    vector<SkewHeap> heap(n);
    for (auto e: edges) 
      heap[e.dst].push(e);

    double score = 0;
    vector<int> seen(n, -1);
    seen[root] = root;
    for (int node = 0; node < n; ++node) { // For each node node
      vector<int> path;
      for (int current = node; seen[current] < 0;) { // Find the cycle

        path.push_back(current);
        seen[current] = node;
        if (heap[current].isEmpty()) return INF; 

        Edge minEdge = heap[current].top(); 
        score += minEdge.weight;
        heap[current].add(-minEdge.weight);
        heap[current].pop();

        int v = unionFind.root(minEdge.src); // Find the root of the other node
        if (seen[v] == node) {
          SkewHeap newHeap;
          while (1) {
            int w = path.back();
            path.pop_back();
            newHeap.merge(heap[w]);
            if (!unionFind.unite(v, w)) break;
          }
          heap[unionFind.root(v)] = newHeap;
          seen[unionFind.root(v)] = -1; // Mark as unseen
        }
        current = unionFind.root(v);
      }
    }
    return score;
  }
};

// Get benchmark time
double currentTime() {
    static clock_t lastTime;
    clock_t currentTime = clock();
    double elapsed = double(currentTime - lastTime) / CLOCKS_PER_SEC;
    lastTime = currentTime;
    return elapsed;
}

// Main function to test the algorithm
int main() {
  for (int q = 0; q <= 100000; ++q) {
    srand(q);

        
        int nodeCount = rand() % 1000 + 1;
        MinimumArborescence solver(nodeCount);
       

        // Adding random edges to the graph
        for (int i = 0; i < nodeCount; ++i) {
            int edgesCount = rand() % nodeCount;
            for (int k = 0; k < edgesCount; ++k) {
                int j;
                do {
                    j = rand() % nodeCount;
                } while (i == j);
                int w = 1 + rand() % 100;
                solver.addEdge(i, j, w);
            }
        }

    double result = solver.solve(0);
    cout << result << " " << currentTime() << endl;
  }
  cout << "*** NO PROBLEM ***" << endl;
}
