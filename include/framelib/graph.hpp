#ifndef GRAPH
#define GRAPH
#include "node.hpp"
#include "time.hpp"
#include "spatioNode.hpp"
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
using namespace std;
class Graph{
public:
    Graph(double pi, int disPre, int maxLength, char* fileName);
    //时空模式专用构造函数，依次传入参与度阈值，隶属度阈值，时间片长度，时间长度阈值，文件名
    Graph(double pi, double c_threshold, int timeSpan, int t_threshold, char* fileName); 
    virtual void algorithm() = 0;
    vector< vector<Node*> > getCliques();
    vector< vector<Node*> > getMaximalCliques();
    static double getPIPre() {return _PIPre;}
    static int getDisPre(){return _disPre;}
    int getMaxLength(){return  _maxLength;}
    vector<Node> getAllNodes(){return _nodes;}
    void printAns(vector< vector<char> > ans);
    static int loadFeatureNum(char c);
    int getModel();
    vector< vector<Node*> > getMOEC(); //2022年论文MOEC专用，一般用不到这个函数   
    static map<char, int> _featureNum;
    bool checkPourn(string pattern);
    static int _timeSpan;//时空模式专用
    static int _t_threshold;//时空模式专用
protected:
    vector<string> result;
    map <Node, vector<Node> > _neiRelationship;
    vector<Node> _nodes;
    vector<vector<string> > _strData;
    static double _PIPre;
    static int _disPre;
    int _maxLength;
    // map<Road*, vector<Road> > neighbor_roadMap; //时空模式专用，保存道路之间的联通关系
    vector<SpatioNode> allInstance; //时空模式专用，保存所有拥堵实例
    vector<Road> allRoad; //时空模式专用，保存所有道路。
};


#endif