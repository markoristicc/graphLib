#include "graph.cpp"
#include <chrono>
#include <limits>

using namespace std; //lol lame

float processArgs(char* argv[], int i) {
        errno = 0;
        char* endptr = nullptr;
        float f = std::strtof(argv[i], &endptr);

        if (endptr == argv[i]) {
            std::cerr << "Bad format: " << argv[i] << "\n"; return 1;
        }
        if (errno == ERANGE) {
            std::cerr << "Out of range: " << argv[i] << "\n"; return 1;
        }
        return f;
}

chrono::nanoseconds timePageRank( pageRankGraph g, float damp, float epsilon) {
    auto t1 = chrono::high_resolution_clock::now();
    volatile vector<float> res = pageRank( g, damp, epsilon);
    auto t2 = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(t2 - t1);
    return elapsed;    
}

vector<chrono::nanoseconds> multiSamplePR( pageRankGraph g, float damp, float epsilon,
                                           int iters) {
    vector<chrono::nanoseconds> res; 
    for(int i = 0; i < iters; i++) { res.push_back( timePageRank(g, damp, epsilon));}
    return res;
}

void sizeScaleTest( float damp, float epsilon, int scaleFactor, int startSize, int iters, int samples) {
    
    for (int i = 0; i < iters; i++) {
        vector<vector<int>> adjList = vector<vector<int>>(startSize, vector<int>(0));
        randomlyPopulate(adjList);
        pageRankGraph g = pageRankGraph( startSize, adjList);
        auto timeList = multiSamplePR(g, damp, epsilon, samples);
        long long avg = 0;
        long long max = 0, min = std::numeric_limits<long long>::max();
        for (auto t: timeList) {
            avg += t.count();
            max = std::max(max, t.count());
            min = std::min(min, t.count());
        }
        avg = avg/timeList.size();
        std::cout<<"Size: "<<startSize<<", Damp: "<<damp<<", Epsilon: "<<epsilon
            <<" PageRank takes an average time of: "<<avg<<" nanoseconds, max time: "<<max
            <<", min time: "<<min<<"\n";
        startSize *= scaleFactor;
    }
}

void epsilonScaleTest( float damp, float startingEpsilon, int scaleFactor, int size, int iters, int samples) {
    // who knows what we boutta do anyway
    for (int i = 0; i < iters; i++) {
        vector<vector<int>> adjList = vector<vector<int>>(size, vector<int>(0));
        randomlyPopulate(adjList);
        pageRankGraph g = pageRankGraph( size, adjList);
        auto timeList = multiSamplePR(g, damp, startingEpsilon, samples);
        long long avg = 0;
        long long max = 0, min = std::numeric_limits<long long>::max();
        for (auto t: timeList) {
            avg += t.count();
            max = std::max(max, t.count());
            min = std::min(min, t.count());
        }
        avg = avg/timeList.size();
        std::cout<<"Size: "<<size<<", Damp: "<<damp<<", Epsilon: "<<startingEpsilon
            <<" PageRank takes an average time of: "<<avg<<" nanoseconds, max time: "<<max
            <<", min time: "<<min<<"\n";
        startingEpsilon = startingEpsilon/scaleFactor;
    }
}

void connectivityScaleTest( float damp, float epsilon, int size, int iters, int samples, int connectivity) {
    // I expect this to remain relatively consistent? We have no sparse implementation so 
    // I would expect the matmuls to all take the same amount of time.
    // unless there is some HW/RT trickery i'm not aware of which optimizes 0*0 matmuls
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <number of nodes> <damp> <epsilon>\n";
        return 1;
    }
    float damp = processArgs(argv, 1);
    float epsilon = processArgs(argv, 2);
    std::cout<<"SIZE SCALE TEST\n";
    sizeScaleTest(damp, epsilon, 2, 128, 5, 30);
    std::cout<<"EPSILON SCALE TEST\n";
    epsilonScaleTest(damp, epsilon, 10, 128, 7, 30);
    return 0;
}

