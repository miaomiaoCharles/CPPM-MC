#include "graph.hpp"
#include "node.hpp"
#include "moec.hpp"
#include "time.hpp"
#include "myAlgorithm.hpp"
#include "myAlgorithm_link.hpp"
#include "myAlgorithm_link2.hpp"
#include <iostream>
using namespace std;
int main(int argc, char **argv){
    unique_ptr<Time> t(new Time());
    Graph* graph = new MyAlgorithm_link2(atof(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4]);
    graph->algorithm();
    cout << "算法花费时间："  << t->getTime() << endl;
    delete graph;
    return 0;
}