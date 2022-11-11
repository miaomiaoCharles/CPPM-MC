#pragma once
#include <iostream>
#include <string>
#include "node.hpp"
#include <vector>
using namespace std;

class Road;
class SpatioNode{
public:
    // SpatioNode(int travelTime);
    SpatioNode(string name);
    string roadName()const;
    string getInsName()const;
    Road* feature;
    int getTimeSpan()const;
    bool operator == (const SpatioNode& node)const;
    bool operator < (const SpatioNode& node)const;
    // bool isCongestion();
private:
    string _roadName;
    int _timeSpan;  //属于的时间片(实例id)
    double _travelTime;  //通过时间
};

class Road{
public:
    Road(string roadName);
    static double _timeSpan;
    bool operator < (const Road& r)const;
    bool operator ==(const Road& r)const;
    // void operator = ( Road& r)const;
    // void addInstance(SpatioNode* ins);
    string name()const;
    vector<SpatioNode*> congestionTimes;
    vector<Road*> _neighbor;
private:
    double _freeV; //自由流速
    string _roadName;  //道路名（特征)
    // vector< Road > _neiborRoad;
};


