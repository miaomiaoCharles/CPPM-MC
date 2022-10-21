#include "myAlgorithm_link2.hpp"




class Row{
public:
    Row(){};
    Row(vector<Node*> v){
        for(int i = 0; i < v.size(); i++){
            _pattern.push_back(v[i]->getFeature());
        }
        sort(_pattern.begin(), _pattern.end());
        for(int i = 0; i < v.size(); i++){
            if(_row.find(v[i]->getFeature()) != _row.end()){
                _row[v[i]->getFeature()] ++;
            }else{
                _row.insert({v[i]->getFeature(), 1});
            }
        }
        
    }
    void operator += (Row& fatherRow){
        for(auto it = _row.begin(); it != _row.end(); it++){
            it->second += fatherRow.getNum(it->first);
        }
    }
    bool isPrevalent(){
        double pi = 1.0;
        for(auto it = _row.begin(); it != _row.end(); it++){
            double pr = it->second/double(Graph::loadFeatureNum(it->first));
            pi = min(pi, pr);
        }
        if(pi >= Graph::getPIPre()){
            return true;
        }else{
            return false;
        }
    }
    vector<Row> porun(){
        vector<Row> ans;
        for(auto missp: _row){
            Row newrow;
            for(auto p: _row){
                if(p != missp){
                    newrow._row.insert(p);
                    newrow._pattern.push_back(p.first);
                }
            }
            ans.push_back(newrow);
        }
        return ans;
    }
    string printRow(){
        string str;
        for(auto it = _row.begin(); it != _row.end(); it++){
            str += it->first;
            str += ":";
            str += to_string(it->second);
            str += " ";
        }
        str+="\n";
        return str;
    }
    int size(){return _row.size();}
    string getPattern(){return _pattern;}
    int getNum(char c){return _row[c];}
    // int id(){return _id;}
private:
    map<char, int>  _row;
    string _pattern;
};

// unordered_map<string, unordered_map<string, bool> > hashMap;


class Group{
public:
    // Group(string pattern):_pattern(pattern){

    // }
    void addNodes(vector<Node*> nodes){
        _nodes.push_back(nodes);
    }
    void setHash(string instance){
        _hash[instance] = true;
    }
    bool isValueInHash(string instance){
        if(_hash.find(instance) == _hash.end() || _hash[instance] == false){
            return false;
        }
        return true;
    }
    vector< vector<Node*> > _nodes;
private:
    // string _pattern;
    vector<Row> _rows;
    unordered_map<string, bool> _hash;
    
};

unordered_map<string, Group> all_groups;


void MyAlgorithm_link2::algorithm() {
    //先进行初始化，建立每个组
    vector< vector<Node*> > maxCliques = getMaximalCliques();
    for(int i = 0; i < maxCliques.size(); i++){
        vector<Node*>& clique = maxCliques[i];
        string str;
        for(Node* n: clique){
            str += n->getFeature();
        }
        if(all_groups.find(str) != all_groups.end()){
            unique_ptr<Group> g(new Group());
            all_groups.insert({str, *g});
            all_groups[str].addNodes(clique);
        }
    }

    //初始化完成后，开始逐个遍历组
    for(auto it = all_groups.begin(); it != all_groups.end(); it++){
        Group& group = it->second;
        for(int i = 0; i < group._nodes.size(); i++){
            vector<Node*>& rowNodes =  group._nodes[i];
            map<char, int> tempm;
            for(auto nodep: rowNodes){
                if(tempm.find(nodep->getFeature()) == tempm.end()){
                    if(!group.isValueInHash(nodep->getName())){
                        tempm.insert({nodep->getFeature(), 1});
                    }else{
                        tempm.insert({nodep->getFeature(), 0});
                    }
                }else{
                   tempm[nodep->getFeature()] ++; 
                }
            }
        }
    }

}