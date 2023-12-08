#include "gabow.cpp"
#include "edmonds.cpp"
#include <cassert>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std;

clock_t lastTime;

void startTimer() {
    lastTime = clock();
}

double stopTimer() {
    return (double)(clock() - lastTime) / CLOCKS_PER_SEC;
}

Graph randomGraph(int nodeCount, int edgeCount) {
  Graph graph(nodeCount);

  graph.edges.reserve(edgeCount*nodeCount);

  // Create a random graph multi-threaded
    vector<thread> threads;
    vector<vector<Edge>> edges(nodeCount);
    
    for (int i = 0; i < nodeCount; ++i) {
        threads.push_back(thread([i, nodeCount, edgeCount, &graph, &edges]() {
            for (int k = 0; k < edgeCount; ++k) {
                int j;
                do {
                    j = rand() % nodeCount;
                } while (i == j);
                int w = 1 + rand() % 100;
                edges[i].push_back({i, j, w});
            }
        }));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < edges[i].size(); ++j) {
            graph.addEdge(edges[i][j].src, edges[i][j].dst, edges[i][j].weight);
        }
    }

    return graph;
}

int main(int argc, char** argv) {
  
    int nodeCount = 0;
    int edgeCount = 0;

    if (argc > 2) {
        nodeCount = atoi(argv[1]);
        edgeCount = atoi(argv[2]);
    } else {
        std::cout << "Usage: " << argv[0] << " <nodeCount> <edgeCount>" << endl;
        return 1;
    }

    Graph graph = randomGraph(nodeCount, edgeCount);

    spaghetti_edmonds edmonds(graph);
    spaghetti_gabow gabow(graph);

    startTimer();
    int edmondsResult = edmonds.findArborescence(0);
    double edmondsTime = stopTimer();
    
    startTimer();
    int gabowResult = gabow.findArborescence(0);
    double gabowTime = stopTimer();

    std::cout << "Edmonds: " << edmondsResult << " " << edmondsTime << endl;
    std::cout << "Gabow: " << gabowResult << " " << gabowTime << endl;
    

  return 0;
}
