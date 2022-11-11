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
    set<string> _str;
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

Table mergeTable(Table& table1, Table& table2){
    Table newTable;
    set<string> newSet(table1._str);
    for(auto it = table2._str.begin(); it != table2._str.end(); it++){
        if(it == table2._str.begin()) continue;
        newSet.insert(*it);
    }
    newTable._str = newSet;
    newTable._size = newSet.size();
    vector<set<SpatioNode > >& _table1 = table1._table;
    vector<set<SpatioNode > >& _table2 = table2._table;
    for(int i = 0; i < _table1.size(); i++){
        for(int j = 0; j < _table2.size(); j++){
            const SpatioNode& node1 = *(_table1[i].begin());
            const SpatioNode& node2 = *(_table2[j].begin());
            if(node1 == node2){
                set<SpatioNode> temp = _table1[i];
                for(auto it = _table2[j].begin(); it != _table2[j].end(); it++){
                    if(it == _table2[j].begin()) continue;
                    temp.insert(*it);
                }
                newTable._table.push_back(temp);
            }
        }
    }
    return newTable;
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
    map< set<string>, Table> tableMap; //存储对应的表实例
    //先计算2阶的情况
    for(auto it = insNeighborMap.begin(); it != insNeighborMap.end(); it++){
        set<string> strSet;
        strSet.insert(it->first->roadName());
        strSet.insert(it->second->roadName());
        if(tableMap.find(strSet) == tableMap.end()){
            Table t;
            t._str = strSet;
            t._size = strSet.size();
        }
        set<SpatioNode> tempSet;
        tempSet.insert(*(it->first));
        tempSet.insert(*(it->second));
        tableMap[strSet]._table.push_back(tempSet);
    }
    int k = 2;
    vector<set<string> > ans;
    vector<Table> prevalentTable;
    for(auto it = tableMap.begin(); it != tableMap.end(); it++){
        if(checkPrivalent(it->second, _PIPre)){
            ans.push_back(it->first);
            prevalentTable.push_back(it->second);
        }
    }//size-2 complete
    bool flag;
    while(1){
        flag = false;
        for(int i = 0; i < prevalentTable.size()-1; i++){
            Table& table1 = prevalentTable[i];
            for(int j = i+1; j < prevalentTable.size(); j++){
                Table& table2 = prevalentTable[j];
                if(*(table1._str.begin()) == *(table2._str.begin())){
                    Table newTable = mergeTable(table1, table2);
                    if(checkPrivalent(newTable, _PIPre)){
                        flag = true;
                        ans.push_back(newTable._str);
                        prevalentTable.push_back(newTable);
                    }
                }
            }
        }
        if(flag == false) break;
    }
    
    for(auto ansSet: ans){
        for(auto it = ansSet.begin(); it != ansSet.end(); it ++){
            cout << * it << " ";
        }
        cout << endl;
    }
    
}