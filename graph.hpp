#include <vector> 
#include <numeric>
#include <stdlib.h>

using namespace std;

class graph {
public:
    int v;
    int e;
    vector<vector<int>> edges;
    graph(){ v = 0; e = 0; edges = vector<vector<int>>(0, vector<int>(0));};
    // add Node v+1 connected to neighbors
    void addNode(vector<int>neighbors);
    // add specific edges to graph
    virtual void addEdges(vector<pair<int, int>> toAdd);
};

class digraph : public graph {
public:
    void addEdges(vector<pair<int,int>> toAdd) override;
};

class pageRankGraph {
public:
    int v, e;
    vector<vector<float>>adjMat;
    vector<int> neighbors;
    pageRankGraph() {v = 0; e = 0;};
    pageRankGraph(graph g) {
        for(int i = 0; i < g.v; i++) {
            for (int j = 0; j < g.edges[i].size(); j++) {
                adjMat[i][g.edges[i][j]] = 1.0/(g.edges[i].size());
            }
        }
        v = g.v, e = g.e; // does this even matter?
        // maybe think about adding init pageRank member vector
    };
    
    pageRankGraph(int nodes, vector<vector<int>> adjList) {
        for(int i = 0; i < nodes; i++) {
            adjMat.push_back(vector<float>(nodes, 0));
        }
        for(int i = 0; i < nodes; i++) {
            for (int j = 0; j < adjList[i].size(); j++) {
                adjMat[i][adjList[i][j]] = 1.0/(adjList[i].size());
            }
            neighbors.push_back(adjList[i].size());
        }
        v = nodes;
        e = std::accumulate(neighbors.begin(), neighbors.end(), 0); 
    }
};
