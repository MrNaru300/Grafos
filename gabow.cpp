/*

    Code based on: Spaghetti Source Code - Gabow's Algorithm
    Original code: https://github.com/spaghetti-source/algorithm/blob/master/graph/arborescence.cc

    
    Minimum Arborescence 

    Description:
      Let G = (V, E) be a weighted directed graph.
      For a vertex r, an edge-set T is called r-arborerscense if
        (1) T is a spanning tree (with forgetting directions),
        (2) for each u in V, indeg_T(u) <= 1, indeg_T(r) = 0.
      The program finds the minimum weight of r-arborescence.

    Algorithm:
      Gabow et al.'s efficient implementation of Chu-Liu/Edmonds.
      Use disjoint set and mergeable heap data structure.

    Complexity: 
      O(m log n)

    Remark:
      To recover the edges, it requires additional works. 
      See Camerini et al.

    References: 
      H. N. Gabow, Z. Galil, T. Spencer, and R. E. Tarjan (1986):
      Efficient algorithms for finding minimum spanning trees in undirected and directed graphs.
      Combinatorica, vol 6, pp. 109--122.

      Y. J. Chu and T. H. Liu (1965): 
      On the shortest arborescence of a directed graph.
      Science Sinica, vol. 14, pp. 1396--1400.

      J. Edmonds (1967): 
      Optimum branchings.
      Journal on Research of the National Bureau of Standards, 71B,
      pp. 233--240.

      P. M. Camerini, L. Fratta, and F. Maffioli (1979):
      A note on finding optimum branchings,
      Networks, vol. 9, pp. 309--312.

*/



#include "graph.h"
#include <vector>

using namespace std;


struct union_find {
  vector<int> nodes; 
  union_find(int n) : nodes(n, -1) { };

  // Unite two sets; return false if they are already in the same set
  bool unite(int node, int other_node) { 
    if ((node = root(node)) == (other_node = root(other_node))) return false; // If they are already in the same set, return false
    
    if (nodes[node] > nodes[other_node]) swap(node, other_node); // Union by rank
    nodes[node] += nodes[other_node]; nodes[other_node] = node; // Update the size of the set
    return true;
  }

    // Check if two elements are in the same set
  bool find(int node, int other_node) { return root(node) == root(other_node); }

  int root(int node) { return nodes[node] < 0 ? node : nodes[node] = root(nodes[node]); }

  
  int size(int node) { return -nodes[root(node)]; }
};

struct skew_heap {
  struct node {
    node *children[2];
    Edge key; // The minimum incoming edge
    double delta; // Lazy update
  } *root;
  skew_heap() : root(0) { }

  // Propagate the lazy updates in the heap
  void propagate(node *a) {
    a->key.weight += a->delta;
    if (a->children[0]) a->children[0]->delta += a->delta;
    if (a->children[1]) a->children[1]->delta += a->delta;
    a->delta = 0;
  }

  // Merge two heaps and return the new root
  node *merge(node *a, node *b) {
    if (!a || !b) return a ? a : b; // If one of the heaps is empty, return the other
    propagate(a); propagate(b); // Propagate the lazy updates
    if (a->key.weight > b->key.weight) swap(a, b); // Make sure that a is the heap with the minimum incoming edge
    a->children[1] = merge(b, a->children[1]); // Merge the heaps
    swap(a->children[0], a->children[1]); // Swap the children
    return a;
  }
  void push(Edge key) {
    node *n = new node(); // Create a new node
    n->children[0] = n->children[1] = 0; // The new node has no children
    n->key = key; n->delta = 0; // Set the key and delta of the new node
    root = merge(root, n); // Merge the new node with the heap
  }
  void pop() {
    propagate(root); // Propagate the lazy updates
    node *temp = root; // Store the root
    root = merge(root->children[0], root->children[1]); // Merge the children of the root
  }
  Edge top() {
     propagate(root); // Propagate the lazy updates
    return root->key; // Return the minimum incoming edge
  }
  bool isEmpty() { 
    return !root;
  }

  void add(double delta) {
    root->delta += delta;
  }

  void merge(skew_heap x) {
    root = merge(root, x.root);
  }
};


struct spaghetti_gabow : public Graph {
  
  void make_graph() {
    n = 0;
    for (auto e: edges) 
      n = max(n, max(e.src, e.dst)+1);
  }

    spaghetti_gabow(Graph graph) : Graph(graph) { 
        make_graph();
    };


/**
 * Finds the arborescence rooted at vertex 'root' in a directed graph.
 * 
 * @param root The root vertex of the arborescence.
 * @return The total weight of the arborescence, or -1 if no arborescence exists.
 */
  int findArborescence(int root) {
    int n = this->n;
    std::vector<Edge>& edges = this->edges;
    union_find unionFind(n);
    vector<skew_heap> heap(n);

    // Add the incoming edges to the heaps
    for (auto e: edges) 
      heap[e.dst].push(e);

    
    int score = 0; // Total weight of the arborescence
    
    vector<int> seen(n, -1); // Seen: -1 = unseen, otherwise = component number
    seen[root] = root; // Mark the root as seen


    for (int node = 0; node < n; ++node) {
      vector<int> path; // The path of nodes to be contracted
      for (int currentNode = node; seen[currentNode] < 0;) { // While the current node is unseen
        path.push_back(currentNode); // Add the current node to the path
        seen[currentNode] = node; // Mark the current node as seen
        if (heap[currentNode].isEmpty()) return -1;  // Return -1 if there is no arborescence

        Edge minimumEdge = heap[currentNode].top(); // Get the minimum incoming edge
        score += minimumEdge.weight; // Add the weight of the minimum incoming edge
        heap[currentNode].add(-minimumEdge.weight); // Cancel the minimum incoming edge
        heap[currentNode].pop(); // Remove the minimum incoming edge

        int v = unionFind.root(minimumEdge.src); // Find the root of the other node
        if (seen[v] == node) { // If the other node is in the cycle
          skew_heap new_heap; // Create a new heap
          int weight; // The weight of the node

          // Merge the heaps of the nodes in the cycle
          do {
            weight = path.back();
            path.pop_back();
            new_heap.merge(heap[weight]);
          } while (unionFind.unite(v, weight));

          // Add the new heap to the root of the cycle
          heap[unionFind.root(v)] = new_heap;
          seen[unionFind.root(v)] = -1;
        }
        // Move to the root of the other node
        currentNode = unionFind.root(v);
      }
    }
    return score;
  }
};
