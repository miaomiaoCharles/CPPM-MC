#pragma once
#include <iostream>
#include"graph.hpp"
using namespace std;

//在这里写自己的自定义类，只需要继承Graph，并且重写algorithm方法即可
class CPPM:public Graph
{
public:
    CPPM(double pi, int disPre, int maxLength, char* fileName):Graph(pi, disPre, maxLength, fileName){};
    void algorithm();
private:

};