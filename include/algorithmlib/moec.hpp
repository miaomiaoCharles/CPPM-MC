#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include "graph.hpp"
class Moec:public Graph
{
public:
    Moec(double pi, int disPre, int maxLength, char* fileName):Graph(pi, disPre, maxLength, fileName){};
    void algorithm();
private:

};
