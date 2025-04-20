#include "graph.hpp"
#include <iostream>
#include <queue>
#include <cstdlib>
#include <math.h>

using namespace std;

void graph::addNode(vector<int> neighbors) {
    for (int i = 0; i < neighbors.size(); i++){
        edges[neighbors[i]].push_back(v);
    }
    edges.push_back(neighbors);
    e += neighbors.size();
    v++;
    // is there anything else we need to do. v is number of nodes. 0 indexing yaayayaay
};

void graph::addEdges(vector<pair<int, int>> toAdd) {
    for (int i = 0; i < toAdd.size(); i++) {
        edges[toAdd[i].first].push_back(toAdd[i].second);
        edges[toAdd[i].second].push_back(toAdd[i].first);
    }
    e += toAdd.size();
};

void digraph::addEdges(vector<pair<int, int>> toAdd) {
    for (int i = 0; i < toAdd.size(); i++) {
        edges[toAdd[i].first].push_back(toAdd[i].second);
    }
    e += toAdd.size();
};  

// ok now some functions for traversing the graph
// return bfs traversal from start node
vector<int> bfs (graph& g, int start) {
    queue<int> Queue = queue<int>();
    Queue.push(start);    
    vector<int> ret = vector<int>(0);
    vector<bool> visited = vector<bool>(g.v, false);
    visited[start] = true;
    while(!Queue.empty()) {
        int top = Queue.front();
        Queue.pop();
        ret.push_back(top);
        for(auto neighbor: g.edges[top]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                Queue.push(neighbor);
            }
        }
    }
    return ret;
}

// return dfs traversal from start node
vector<int> dfs (graph& g, int start) {
    vector<bool> visited = vector<bool>(g.v, false);
    vector<int> ret = vector<int>(0);
    std::function<void(int)> helper = [&](int start) {
        for(int i = 0; i < g.edges[start].size(); i++) {
            if (!visited[g.edges[start][i]]) {
                visited[g.edges[start][i]] = true;
                ret.push_back(g.edges[start][i]);
                helper(g.edges[start][i]);
            }
        }
    };
    ret.push_back(start);
    visited[start] = true;
    helper(start);
    return ret;
}

vector<pair<int,int>> kruskals (graph& g) {
    // meow meow meow meow meow meow meow
    // how many meows does a motherfucker need to do
    return {{}};
}

// ok let's go back to basics
// naive def
// R(u) = c * SUM( R(v)/Nv) ) over all v which point to u. (Nv = number of links from v)
// c is normalization factor s.t. total rank of all pages is constant. (???)
// c < 1 due to some pages not having forward links
//
// let A be modified adj matrix s.t. A (u,v) = 1/N(u) iff E (u,v) in edges, 0 else
// if we treat R as a vector over the web pages, then have R = cAR -> R is eigenvector of A
// w/ eigenvalue c. In fact, we want dominant eigenvector of A, which can be computed by repeatedly
// applying A to any nondegenerate start vector. 
// ok question: doesn't this mean AR = 1/c * R -< 1/c is eigenvalue, not c?
// ok, so basically due to spectral graph theory, this network converges
// r0 <- any random vector over nodes (v dim)
// r(i+1) <- Ari
// d <- l1 norm ri - ri+1 l1 norm
// ri+1 <- ri+1 + dE
// & <- l1 norm ri1 - ri
// while & > epsilon (some arbitrary stopping point?)
float norml1( std::vector<float> a) {
    float ret = 0;
    for (int i = 0; i < a.size(); i++) {
        ret += abs(a[i]);
    }
    return ret;
}

template<typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector sizes must match for subtraction");
    }
    std::vector<T> result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] - b[i]);
    }
    return result;
};

template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector sizes must match for subtraction");
    }
    std::vector<T> result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] + b[i]);
    }
    return result;
};

vector<float> scalarVector( float c, vector<float> v) {
    vector<float> res; // could this be faster by using accum? we'll see.
    for( auto a: v) {
        res.push_back(a*c);
    }
    return res;
}

vector<float> vectorMatmul( vector<vector<float>> A, vector<float> v) {
    // meow meow meow;
    vector<float> res = vector<float>( v.size(), 0);
    for( int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A.size(); j++) {
            res[i] += v[j] * A[j][i];
        }
    }
    // naive implementation for now
    return res;
};

vector<float> pageRankGPT(vector<vector<float>> adj,
                          float damp   = 0.85f,
                          float eps    = 1e-6f)
{
    const size_t n = adj.size();
    if (n == 0 || adj[0].size() != n)
        throw std::invalid_argument("Adjacency matrix must be non‑empty and square");

    vector<float> r0(n, 1.0f / n);          // r₀
    const vector<float> randomJump(n, 1.0f / n); // (1/N)·1
    vector<float> next;
    while (true) {
        // r_{k+1} = d · r_k · P + (1‑d) · (1/N)·1
        next = scalarVector(damp, vectorMatmul(adj, r0)) +
               scalarVector(1.0f - damp, randomJump);

        if (norml1(next - r0) < eps) break;      // converged
        r0.swap(next);
    }
    return r0;                                   // Σ rank == 1
}

vector<float> pageRank( pageRankGraph& g, float damp, float epsilon) {
    //A is already initialized accordingly
    return pageRankGPT( g.adjMat, damp, epsilon);
    vector<float> r0 = vector<float>(g.v, 0.25);
    vector<float> r1 = vector<float>(g.v, 0.5);
    float d = damp; // not sure here what to do
    vector<float> E = vector<float>(g.v, 0.25); // E and r0 are same initially
    while( norml1( r1 - r0) > epsilon) {
        r1 = vectorMatmul( g.adjMat, r0);
        d = norml1(r0) + norml1(r1);
        r1 = r1 + scalarVector(d, E);
    }
    return r1;
}

void randomlyPopulate (vector<vector<int>> &adjList) {
    // ok so how do we want to do this. adjList is of dim (v, 0)
    // generate random numbers in [1,n], add to previous number. 
    int n = adjList.size();
    int random = 0;
    for( int i = 0; i < n; i++) {
        auto& v = adjList[i];
        while (random < n) {
            random += std::rand()%(n/3) + 2;
            if (i == random && v.empty()) random = (i-1)%n;
            if (i == random || random >= n) break;
            v.push_back(random); 
        }
        random = 0;
    }
}

void printMatrix(std::vector<std::vector<float>> mat, int width = 10, int precision = 4) {
    for (const auto& row : mat) {
        for (float val : row) {
            std::cout << val <<" ";
        }
        std::cout << '\n';
    }
}

int main() {
    int v;
    cout<<"Input graph size. Edges will be randomly constructed\nnumber of nodes: ";
    cin>>v;
    cout<<"\n";
    vector<vector<int>> adjList = vector<vector<int>>(v, vector<int>(0));
    // how to randomly generate?
    randomlyPopulate(adjList);
    pageRankGraph g = pageRankGraph(v, adjList);
    printMatrix(g.adjMat);
    std::cout<<"enter damping factor: ";
    float damp = 0, epsilon = 0;
    std::cin>>damp;
    std::cout<<"enter epsilon: ";
    std::cin>>epsilon;
    vector<float> res = pageRank( g, damp, epsilon);
    return 0;
}
