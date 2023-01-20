#include "graph.hpp"
#include "node.hpp"
#include "time.hpp"
#include "CPPM-MC.hpp"
#include "MaxIns.hpp"
#include <iostream>
using namespace std;
int main(int argc, char **argv){
    
    Graph* graph = new CPPM(atof(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4]);
    // Graph* graph = new MySPCP(atof(argv[1]), atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    graph->algorithm();
   
    delete graph;
    return 0;
}