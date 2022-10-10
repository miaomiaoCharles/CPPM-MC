#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;
 //每一个行实例的节点，格式  A<1,3>
class RowNode{  
public:
    RowNode(char feature, vector<int> ins);
    int getInsSize()const{return _ins.size();}
    bool operator < (const RowNode& n)const;
    bool operator == (const RowNode& n)const;
    char getFeature()const;
    const vector<int> getIns()const;  //获取ins，但是只能读
    void addIns(const vector<int>& fatherNode_ins);
    vector<bool> _diffHash;  //去避免实例重复计数的哈希表，每个rowNode都对应一个，默认的最大特征数量是255
private:
    char _feature;
    vector<int> _ins;
};