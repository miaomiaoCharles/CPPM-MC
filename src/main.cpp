#include "graph.hpp"
#include "node.hpp"
#include "moec.hpp"
#include "myAlgorithm.hpp"
#include "myAlgorithm_link.hpp"
#include <iostream>
using namespace std;
int main(int argc, char **argv){
    Graph* graph = new MyAlgorithm_link(atof(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4]);
    graph->algorithm();
    delete graph;
    return 0;
}