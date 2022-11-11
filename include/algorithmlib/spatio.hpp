#pragma once
#include <iostream>
#include"graph.hpp"
#include "spatioNode.hpp"
using namespace std;

//在这里写自己的自定义类，只需要继承Graph，并且重写algorithm方法即可
class Spatio:public Graph
{
public:
    Spatio(double pi, double c_threshold, int timeSpan, int t_threshold, char* fileName):Graph(pi, c_threshold, timeSpan, t_threshold, fileName){};
    void algorithm();
private:
};