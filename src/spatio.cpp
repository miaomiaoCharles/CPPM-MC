#include "spatio.hpp"

 map<string, int> featureNum;  //存储实例个数
bool checkNeighbor(SpatioNode& node1, SpatioNode& node2){
    auto it = find(node1.feature->_neighbor.begin(), node1.feature->_neighbor.end(), node2.feature);
    if(it != node1.feature->_neighbor.end()){
        if(abs(node1.getTimeSpan()-node2.getTimeSpan()) * Graph::_timeSpan <= Graph::_t_threshold){
            return true;
        }
    }
    return false;
}
struct Table
{
    int _size;
    string _str;
    vector<set<SpatioNode > > _table;
};


bool checkPrivalent(Table t, double piPre){
    // map<string, int> counter;
    map<string, set<string> > counter;
    for(auto it = t._table.begin(); it != t._table.end(); it++){
        set<SpatioNode>& s = *it;
        for(auto sit = s.begin(); sit != s.end(); sit++){
            counter[sit->roadName()].insert(sit->getInsName());
        }
    }
    double pi = 1.0;
    for(auto it = counter.begin(); it != counter.end(); it++){
        double pr = it->second.size()/featureNum[it->first];
        pi = min(pi, pr);
    }
    if(pi >= piPre){
        return true;
    }else{
        return false;
    }
}


void Spatio::algorithm(){
    //首先先产生所有的临近关系
    map<SpatioNode*, SpatioNode*> insNeighborMap; 
    for(SpatioNode& insNode1: allInstance){
        for(SpatioNode& insNode2: allInstance){
            if(!(insNode1 == insNode2) && checkNeighbor(insNode1, insNode2)){
                insNeighborMap.insert({&insNode1, &insNode2});
            }
        }
    }
   
    for(auto ins: allInstance){
        if(featureNum.find(ins.roadName()) == featureNum.end()){
            featureNum.insert({ins.roadName(), 1});
        }else{
            featureNum[ins.roadName()] ++;
        }
    }
    map<string, Table > tableMap; //存储对应的表实例
    //先计算2阶的情况
    for(auto it = insNeighborMap.begin(); it != insNeighborMap.end(); it++){
        string str;
        str += it->first->roadName();
        str += it->second->roadName();
        if(tableMap.find(str) == tableMap.end()){
            Table t;
            t._str = str;
            t._size = str.size();
        }
        set<SpatioNode> tempSet;
        tempSet.insert(*(it->first));
        tempSet.insert(*(it->second));
        tableMap[str]._table.push_back(tempSet);
    }
    int k = 2;
    // while(){
    //     map<string, Table > newTableMap;
    //     for(auto it = tableMap.begin(); it != tableMap.end(); it ++){
    //         string sorceNodeStr = it->first;
    //         char sorceChar = sorceNodeStr[]
    //     }
    // }

    vector<string> ans;
    for(auto it = tableMap.begin(); it != tableMap.end(); it++){
        if(checkPrivalent(it->second, _PIPre)){
            ans.push_back(it->first);
        }
    }
    for(auto a: ans){
        cout << a << endl;
    }
    
}