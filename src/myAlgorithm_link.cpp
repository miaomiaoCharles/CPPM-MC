#include "myAlgorithm_link.hpp"




class Row{
public:
    Row(vector<Node*> v){
        for(int i = 0; i < v.size(); i++){
            if(_row.find(v[i]->getFeature()) != _row.end()){
                _row[v[i]->getFeature()] ++;
            }else{
                _row.insert({v[i]->getFeature(), 1});
            }
        }
    }
    int size(){return _row.size();}
    // int id(){return _id;}
private:
    map<char, int>  _row;
    int _id;
};

int check(int sorceRowLoc, vector<Row>& rows){
    Row& row = rows[sorceRowLoc];
    
}

void MyAlgorithm_link::algorithm()
{
    vector<vector<Node *> > maxCliques = getMaximalCliques();
    //开始遍历每一行
    map<int, vector<Row> > all_row; //分组，key是组的阶数，value存放所有的对应阶数的row
    //开始对每一个极大团放入对应的分组里去
    for(vector<Node*> clique: maxCliques){
        unique_ptr <Row> rowPtr(new Row(clique));
        int size = rowPtr->size();
        if(all_row.find(size) == all_row.end()){
            all_row.insert({size, {*rowPtr}});
        }else{
            all_row[size].push_back(*rowPtr);
        }
    }
    //正式开始算法，对每个组的每一个row进行遍历
    for(auto it = all_row.begin(); it != all_row.end(); it++){
        int itSize = it->first;
        for(int i = 0; i < it->second.size(); i++){
            // Row row = it->second[i];
            if(check(i, it->second) != -1){

            }
        }
    }
    for(auto a: ans){
        cout << a << endl;
    }
}
