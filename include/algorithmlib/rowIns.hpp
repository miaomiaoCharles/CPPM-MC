#pragma once
#include <iostream>
#include "graph.hpp"
#include "node.hpp"
#include "rowNode.hpp"
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
using namespace std;
//行实例，格式  A:<1,2>, B<1>, C<1>, 其中的每个B<1>这种的都是一个RowNode对象
class RowIns{
public:
    RowIns(vector<Node*> clique);
    RowIns();
    int size()const;
    bool operator < (const RowIns& row)const;
    bool operator == (const RowIns& row);
    string getPattern()const;  //获取对应的模式
    bool isPrevalent()const;    //获取这个模式是否频繁
    int getId()const ;
    string printStr()const; //返回直观的RowIns字符串形式
    void setId(int id);
    bool operator | (RowIns* fatherP); //判断与候选父节点是否每一个node的ins都不同，从而筛选出真正的父节点,使用这个的前提是已经通过了第一步筛选
    const vector<RowNode> getNodeVec();  //仅仅用于判断ins的不同
    void refersh_minCounter();
    // RowNode& get_map(char c)const;
    // unordered_map<char, vector<bool> > _map; //仅仅用于isdifferent函数判断不同
    void add_father(RowIns& fatherIns);
private:
    set<RowNode> _row;
    int _minCounter;
    int _id;
    
    
};