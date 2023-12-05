#include "gabow.cpp"
#include "edmonds.cpp"
#include <cassert>
#include <cstdio>
#include <ctime>

clock_t lastTime;

double startTimer() {
    clock_t lastTime = clock();
    return double(clock() - lastTime) / CLOCKS_PER_SEC;
}

double stopTimer() {
    clock_t currentTime = clock();
    return  double(currentTime - lastTime) / CLOCKS_PER_SEC;
}


int main() {
  
  srand(time(NULL));
  for (int q = 0; q <= 1000; ++q) {

    int nodeCount = rand() % 10000 + 1;
    Graph graph(nodeCount);

    // Adding random edges to the graph
    for (int i = 0; i < nodeCount; ++i) {
        int egdeCount = rand() % 100;
        for (int k = 0; k < egdeCount; ++k) {
            int j;
            do {
                j = rand() % nodeCount;
            } while (i == j);
            int w = 1 + rand() % 100;
            graph.addEdge(i, j, w);
        }
    }

    startTimer();
    
    Edmonds edmonds(graph);
    double edmondsTime = stopTimer();
    double edmondsScore = edmonds.findArborescence(0);
    
    startTimer();
    
    MSAGabow gabow(graph);
    double gabowTime = stopTimer();
    double gabowScore = gabow.findArborescence(0);
    
    printf("Test %d: Edmonds %.3lf %.3lf | Gabow %.3lf %.3lf\n", q, edmondsTime, edmondsScore, gabowTime, gabowScore);
    assert(edmondsScore == gabowScore);
  }


  return 0;
}