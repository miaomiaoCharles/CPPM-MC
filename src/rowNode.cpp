#include "rowNode.hpp"
//rowNode的构造函数，主要是初始化每个节点对应的哈希表。
RowNode::RowNode(char feature, vector<int> ins):_feature(feature), _ins(ins){
    _diffHash.resize(255);
    for(int i = 0; i < _diffHash.size();i++){
        _diffHash[i] = false;
    }
    for(auto i: ins){
        _diffHash[i] = true;
    }
}
bool RowNode::operator < (const RowNode& n)const{
    return this->_feature < n._feature;
}

bool RowNode::operator == (const RowNode& n)const{
    return (this->_feature == n._feature) && (this->_ins == n._ins);
}

char RowNode::getFeature()const{
    return _feature;
}

const vector<int> RowNode:: getIns()const{
    return _ins;
}

void RowNode::addIns(const vector<int>& fatherNode_ins){
    for(int i = 0; i < fatherNode_ins.size(); i++){
        this->_ins.push_back(fatherNode_ins[i]);
    }
}