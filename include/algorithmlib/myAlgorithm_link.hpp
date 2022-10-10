#ifndef MYALGORITHMLINKHPP
#define MYALGORITHMLINKHPP
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <list>
#include "graph.hpp"
#include "rowIns.hpp"
#include "rowNode.hpp"

using namespace std;
//在这里写自己的自定义类，只需要继承Graph，并且重写algorithm方法即可
class MyAlgorithm_link:public Graph
{
public:
    MyAlgorithm_link(double pi, int disPre, int maxLength, char* fileName):Graph(pi, disPre, maxLength, fileName){};
    void algorithm();
    vector<RowIns> all_RowIns;   //用一个vector来存放所有的RowInS;
    // unordered_map<char, set<int> > quickLocation; //用来快速定位RowIns的超集
    unordered_map<int, RowIns*> idToRowInsMap; //通过id找到Rowins的地址
    set<string> ans;  //最终的频繁模式
    // bool check(string pattern); //剪枝系统
private:

};

#endif