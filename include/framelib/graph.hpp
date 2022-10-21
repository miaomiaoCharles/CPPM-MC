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
private:
    map <Node, vector<Node> > _neiRelationship;
    vector<Node> _nodes;
    vector<vector<string> > _strData;
    static double _PIPre;
    static int _disPre;
    int _maxLength;
   
};


#endif