#ifndef MYALGORITHMLINKHPP
#define MYALGORITHMLINKHPP
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <list>
#include "graph.hpp"

using namespace std;
//在这里写自己的自定义类，只需要继承Graph，并且重写algorithm方法即可
class MyAlgorithm_link:public Graph
{
public:
    MyAlgorithm_link(double pi, int disPre, int maxLength, char* fileName):Graph(pi, disPre, maxLength, fileName){};
    void algorithm();
private:

};

#endif