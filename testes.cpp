#include "gabow.cpp"
#include "edmonds.cpp"
#include <cassert>
#include <cstdio>
#include <ctime>
#include <cstdlib>

clock_t lastTime;

void startTimer() {
    lastTime = clock();
}

double stopTimer() {
    return (double)(clock() - lastTime) / CLOCKS_PER_SEC;
}


int main() {
  
  
  for (int q = 0; q <= 100; ++q) {
    srand(q);
    int nodeCount = rand() % 1000 + 1;
    Graph graph(nodeCount);
    int totalWeight = 0;

    printf("Test %d ", q);
    // Adding random edges to the graph
    for (int i = 0; i < nodeCount; ++i) {
        int egdeCount = rand() % 1000;
        for (int k = 0; k < egdeCount; ++k) {
            int j;
            do {
                j = rand() % nodeCount;
            } while (i == j);
            int w = 1 + rand() % 100;
            totalWeight += w;
            graph.addEdge(i, j, w);
        }
    }

    printf("(n = %d, avg deg = %.03f, w = %d): \n", nodeCount, (float)graph.edges.size() / nodeCount, totalWeight);
    
    spaghetti_edmonds edmonds(graph);
    startTimer();
    int edmondsScore = edmonds.findArborescence(0);
    double edmondsTime = stopTimer();
    
    printf("Edmonds %.3lfs %d\n", edmondsTime, edmondsScore);
    
    spaghetti_gabow gabow(graph);
    startTimer();
    int gabowScore = gabow.findArborescence(0);
    double gabowTime = stopTimer();
    
    printf("Gabow %.3lfs %d\n", gabowTime, gabowScore);
    printf("-----------------\n\n");
    
  }


  return 0;
}