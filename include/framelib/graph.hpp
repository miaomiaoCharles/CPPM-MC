#ifndef GRAPH
#define GRAPH
#include "node.hpp"
#include "time.hpp"
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
    void printAns(vector< vector<string> > ans);
    static int loadFeatureNum(string c);
    int getModel(); 
    static map<string, int> _featureNum;
    bool checkPourn(string pattern);
protected:
    vector<string> result;
    map <Node, vector<Node> > _neiRelationship;
    vector<Node> _nodes;
    vector<vector<string> > _strData;
    static double _PIPre;
    static int _disPre;
    int _maxLength;
};


#endif