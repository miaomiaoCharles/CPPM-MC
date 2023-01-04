#include "CPPM-MC.hpp"
#include <unordered_set>
#include <string>
using namespace std;
vector<string> ans;

struct Group{
public:
    using counterList = unordered_map<string, set<int> >;
    Group(set<string> patterns):_patterns(patterns){
        _pattern.clear();
        for(auto pattern: patterns){
            _pattern += pattern;
            double pre = Graph::_featureNum[pattern]*(Graph::getPIPre()*1.0);
            _pre.insert({pattern, pre});
            _counter.insert({pattern,{}});
            _storage.clear();
        }
    }
    Group() = default;
    void addClique(vector<Node*> clique){
        _nodes.push_back(clique);
    }
    int nameSize(){return _patterns.size();}
    string patternStr(){return _pattern;}
    bool checkGroupPrevalent(){
        for(int i = 0; i < _nodes.size(); i++){
            if(_storage.size() == nameSize()) break;
            vector<Node*>& row = _nodes[i];
            for(int j = 0; j < row.size(); j++){
                const string& nodeName = row[j]->getName();
                const string& featureName = row[j]->getFeature();
                if(_storage.find(featureName) != _storage.end()) continue;
                _counter[featureName].insert(row[j]->getNum());
                if(_counter[featureName].size() >= _pre[featureName]){
                    _storage.insert(featureName);
                }
            }
        }
        return _storage.size() == nameSize();
    }  
    //拆分_counter，该函数要在判断该group不频繁之后调用
    vector<counterList> counterSplite(){
        vector<counterList> res;
        for(int i = 0; i < _counter.size(); i++){
            counterList newCounter;
            int tempcounter = 0;
            for(auto it = _counter.begin(); it != _counter.end(); it++){
                if(tempcounter++ == i) continue;
                newCounter.insert(*it);
            }
            res.push_back(newCounter);
        }
        return res;
    }
    //从超集分割下来的，继承这个超集分割的_counter,该函数必须在没有遍历到该组时候调用
    void addFatherResource(counterList counter)
    {
        _counter = counter;
    }
    
private:
    string _pattern;
    set<string> _patterns;
    // unordered_set<string> _hash; //避免相同实例重复的参与度计算
    vector<vector<Node*> > _nodes; //表实例
    counterList _counter; //每个特征进行下标累加，存储累加的下标
    unordered_map<string, double> _pre; //每个特征需要达到的最小数量
    unordered_set<string> _storage; //存放已达到标准的特征，不一定准确！超集关系
}; 

string getPatternFromNodes(vector<Node*> nodes){
    string ans;
    set<string> temp;
    for(auto node: nodes){
        temp.insert(node->getFeature());
    }
    for(auto c: temp){
        ans += c;
    }
    return ans;
}

set<string> getPatternSetFromNodes(vector<Node*> nodes){
    set<string> ans; 
    for(auto node: nodes){
        ans.insert(node->getFeature());
    }
    return ans;
}
struct Cmp{
    bool operator()(const string& a, const string& b){
        if(a.size() != b.size()){
            return a.size() > b.size();
        }else{
            return a < b;
        }
    }
};
map<string, Group, Cmp> all_groups;
unordered_map<string, double> _pre; //每个特征需要达到的最小数量
void CPPM::algorithm(){
    vector< vector<Node*> > maxCliques = getMaximalCliques();
    // for(int i = 0; i < maxCliques.size(); i++){
    //     for(int j = 0; j < maxCliques[i].size(); j++){
    //         cout << maxCliques[i][j]->getName() << " ";
    //     }
    //     cout << endl;
    // }
    for(int i = 0; i < maxCliques.size(); i++){
        vector<Node*>& clique = maxCliques[i];
        string str = getPatternFromNodes(clique);
        set<string> patterns = getPatternSetFromNodes(clique);
        if(patterns.size() < 2) continue;
        if(all_groups.find(str) == all_groups.end()){
            Group* group = new Group(patterns);
            all_groups.insert({str, *group});
        }
        all_groups[str].addClique(clique);
    }
    for(auto it = all_groups.begin(); it != all_groups.end(); it++){
        //检查是否有超集已经加入答案中
        bool flag = false;
        for(int i = 0; i < ans.size(); i++){
            string& str = ans[i];
            if(str.find(it->first) != str.npos){
                flag = true;
                break;
            }
        }
        if(flag) continue;

        Group& group = it->second;
        bool isPrevalent = group.checkGroupPrevalent();
      
        if(isPrevalent){
            //加入结果数组
            ans.push_back(group.patternStr());
        }else{
            //counter拆分
            using counterList = unordered_map<string, set<int> >;
            if(group.nameSize() <= 2) continue;
            vector<counterList> counters = group.counterSplite();
            //遍历每个拆分下来的counter，并把它们安置到对应的组里去
            for(counterList& counter: counters){
                string patternName;
                set<string> patterns;
                for(auto it = counter.begin(); it != counter.end(); it++){
                    patterns.insert(it->first);
                }
                for(auto str: patterns){
                    patternName += str;
                }
                if(all_groups.find(patternName) == all_groups.end()){
                    //如果要安置的组不存在，建立这个组
                    Group* group = new Group(patterns);
                    group->addFatherResource(counter);
                    all_groups.insert({patternName, *group});
                }else{
                    all_groups[patternName].addFatherResource(counter);
                }
            }
        }
    }
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << endl;
    }
}