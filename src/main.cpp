#include "graph.hpp"
#include "node.hpp"
#include "moec.hpp"
#include "time.hpp"
#include "myAlgorithm.hpp"
#include "mySPCP.hpp"
#include "myAlgorithm_link2.hpp"
#include"spatio.hpp"
#include <iostream>
using namespace std;
int main(int argc, char **argv){
    unique_ptr<Time> t(new Time());
    // Graph* graph = new MyAlgorithm_link2(atof(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4]);
    Graph* graph = new Spatio(atof(argv[1]), atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    graph->algorithm();
    cout << "算法花费时间："  << t->getTime() << endl;
    delete graph;
    return 0;
}