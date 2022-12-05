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
    Row(map<char,int> m, string pattern){
        _pattern = pattern;
        _row = m;
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
    // vector<Row> porun(){
    //     vector<Row> ans;
    //     for(auto missp: _row){
    //         Row newrow;
    //         for(auto p: _row){
    //             if(p != missp){
    //                 newrow._row.insert(p);
    //                 newrow._pattern.push_back(p.first);
    //             }
    //         }
    //         ans.push_back(newrow);
    //     }
    //     return ans;
    // }
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
    Group() = default;
    void addNodes(vector<Node*> nodes){
        _nodes.push_back(nodes);
    }
    void setHash(string instance){
        _hash[instance] = true;
    }
    bool isValueInHash(string instance){
        // for(auto it = _hash.begin(); it != _hash.end(); it++){
        //     cout << it->first << " "  << it->second << endl;
        // }
        if(_hash.find(instance) == _hash.end() || _hash[instance] == false){
            return false;
        }
        return true;
    }
    vector< vector<Node*> > _nodes;
    vector<Row> _rows;
private:
    // string _pattern;
    unordered_map<string, bool> _hash;
};
struct cmp{
    bool operator()(const string& a, const string& b){
        if(a.size() > b.size()){
            return true;
        }else if(a.size() < b.size()){
            return false;
        }else{
            return a < b;
        }
    }
};
map<string, Group, cmp> all_groups;

string getPatternFromNodes(vector<Node*> nodes){
    string ans;
    set<char> temp;
    for(auto node: nodes){
        temp.insert(node->getFeature());
    }
    for(auto c: temp){
        ans += c;
    }
    return ans;
}

void MyAlgorithm_link2::algorithm() {
    //先进行初始化，建立每个组,并把每个极大团加入组中
    vector< vector<Node*> > maxCliques = getMaximalCliques();
    for(int i = 0; i < maxCliques.size(); i++){
        for(int j = 0; j < maxCliques[i].size(); j++){
            cout << maxCliques[i][j]->getName() << " ";
        }
        cout << endl;
    }
    for(int i = 0; i < maxCliques.size(); i++){
        vector<Node*>& clique = maxCliques[i];
        string str = getPatternFromNodes(clique);
        if(str.size() < 2) continue;
        all_groups[str].addNodes(clique);
    }
    //初始化完成后，开始逐个遍历组
    for(auto it = all_groups.begin(); it != all_groups.end(); it++){
        Group& group = it->second;
        string pattern = it->first;
        if(checkPourn(pattern)) continue;//剪枝
        for(int i = 0; i < group._nodes.size(); i++){
            vector<Node*>& rowNodes =  group._nodes[i];
            map<char, int> tempm;
            for(auto nodep: rowNodes){
                if(tempm.find(nodep->getFeature()) == tempm.end()){
                    if(!group.isValueInHash(nodep->getName())){  //表示没有在哈希表中存在
                        tempm.insert({nodep->getFeature(), 1});
                        group.setHash(nodep->getName());
                    }else{
                        tempm.insert({nodep->getFeature(), 0});
                    }
                }else{
                   tempm[nodep->getFeature()] ++; 
                }
            }
            //接下来，把tempm转化为Row
            group._rows.push_back(Row(tempm, it->first));
        }
        //接下来，根据row判断逐行频繁是否频繁
        int i = 0;
        for(i = 0; i < group._rows.size(); i++){
            if(i > 0){
                group._rows[i] += group._rows[i-1];
            }
            Row& row = group._rows[i];
            if(row.isPrevalent() == true){
                result.push_back(row.getPattern());
                break;
            }
        }
        //这一组所有row都遍历完了，仍然没有达成频繁的要求，将团拆分并加入对应分组里去
        if(i == group._rows.size() && it->first.size() > 2){
            for(int i = 0;i < group._nodes.size();i++){
                vector<Node*>& fatherV = group._nodes[i];
                if(fatherV.size() < 3) continue;
                for(int j = 0; j < fatherV.size(); j++){
                    Node* missNodep = fatherV[j];
                    vector<Node*> childV;
                    for(int k = 0; k < fatherV.size(); k++){
                        if(fatherV[k] != missNodep) childV.push_back(fatherV[k]);
                    }
                    string childPattern = getPatternFromNodes(childV);
                    if(childPattern.size() < 2|| childV.size() < 2) continue;
                    if(all_groups.find(childPattern) != all_groups.end()){
                        // all_groups[childPattern].addNodes(childV);
                    }else{
                        unique_ptr<Group> p(new Group());
                        all_groups.insert({childPattern, *p});
                        all_groups[childPattern].addNodes(childV);
                    }
                    
                }
            }
        }
    }
    for(auto a: result){
        cout << a << endl;
    }

}