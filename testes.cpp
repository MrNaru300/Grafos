#include "gabow.cpp"
#include "edmonds.cpp"
#include <cassert>
#include <cstdio>
#include <ctime>

clock_t lastTime;

double startTimer() {
    lastTime = clock();
    return double(clock() - lastTime) / CLOCKS_PER_SEC;
}

double stopTimer() {
    clock_t currentTime = clock();
    return  double(currentTime - lastTime) / CLOCKS_PER_SEC;
}


int main() {
  
  srand(time(NULL));
  for (int q = 0; q <= 1000; ++q) {

    int nodeCount = rand() % 1000 + 1;
    Graph graph(nodeCount);
    int totalWeight = 0;

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

    printf("Test %d (n = %d, avg deg = %.03f, w = %d): \n", q, nodeCount, (float)graph.edges.size() / nodeCount, totalWeight);
    
    Edmonds edmonds(graph);
    startTimer();
    int edmondsScore = edmonds.findArborescence(0);
    double edmondsTime = stopTimer();
    
    printf("Edmonds %.3lf %d\n", edmondsTime, edmondsScore);
    
    MSAGabow gabow(graph);
    startTimer();
    int gabowScore = gabow.findArborescence(0);
    double gabowTime = stopTimer();
    
    printf("Gabow %.3lf %d\n", gabowTime, gabowScore);
    printf("-----------------\n\n");
    
  }


  return 0;
}