A graph library written in cpp, implementing a variety of algorithms. 
As of now, the algorithms supported are:
- bfs
- dfs
- pageRank 

I'll add more soon, however at the moment I'm really focused on pageRank.

## PageRank Performance Documentation

My first pass implementation of the pageRank algorithm has the following performance characteristics

```
SIZE SCALE TEST
Size: 128, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 1866270 nanoseconds, max time: 3313166, min time: 1394916
Size: 256, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 4175091 nanoseconds, max time: 5024916, min time: 3974416
Size: 512, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 14921890 nanoseconds, max time: 15233459, min time: 14823792
Size: 1024, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 56384411 nanoseconds, max time: 57464000, min time: 55733041
Size: 2048, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 321256472 nanoseconds, max time: 329200958, min time: 287037916
EPSILON SCALE TEST
Size: 128, Damp: 0.9, Epsilon: 0.01 PageRank takes an average time of: 1106066 nanoseconds, max time: 1158167, min time: 1077459
Size: 128, Damp: 0.9, Epsilon: 0.001 PageRank takes an average time of: 1702538 nanoseconds, max time: 1838833, min time: 1677042
Size: 128, Damp: 0.9, Epsilon: 0.0001 PageRank takes an average time of: 2087974 nanoseconds, max time: 2151292, min time: 2058792
Size: 128, Damp: 0.9, Epsilon: 1e-05 PageRank takes an average time of: 2700689 nanoseconds, max time: 2767334, min time: 2648667
Size: 128, Damp: 0.9, Epsilon: 1e-06 PageRank takes an average time of: 3479462 nanoseconds, max time: 3588625, min time: 3427500
Size: 128, Damp: 0.9, Epsilon: 1e-07 PageRank takes an average time of: 3681094 nanoseconds, max time: 3784083, min time: 3626834
Size: 128, Damp: 0.9, Epsilon: 1e-08 PageRank takes an average time of: 5073172 nanoseconds, max time: 5195583, min time: 5001000
```

These graphs' average number of outgoing connections per node was 3. 

Much more work is to be done in optimizing this implementation. In fact I have done none thus far.

This is because I am lazy.

In all seriousness, I need to update the infrastructure to provide for easier testing, visualization, and command line interaction, 
which is not particularly exciting to me - the performance work is the fun stuff! However, I would also like to have comprehensive documentation 
on the performance gains achieved by my speedups VS a naive implementation, so the performance work will have to wait :(

Anyways, to build the project, run something like the following from root proj directory
```
clang++ --std=c++17 -g testBench.cpp -o pr
```
usage is:
```
./pr <damp> <epsilon>
```
It will perform a size scale test and an epsilon scaling test. If you want to change the graph size, or number of sampling iterations, then be a big boy/girl and change the code yourself.

