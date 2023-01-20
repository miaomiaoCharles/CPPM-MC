#include "CPPM-MC.hpp"
#include <unordered_set>
#include <string>
#include "write.hpp"
using namespace std;
vector<string> ans;
//表实例类
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
      //拆分下来的哈希表，是不一定有表实例对应的，可能一个行实例都没有，但是多次拆分累加后的哈希表导致该模式最终频繁了。
      if(_nodes.size() == 0){
        for(auto it = _counter.begin(); it != _counter.end(); it++){
          if(it->second.size() >= _pre[it->first]){
            _storage.insert(it->first);
          }
        }
        return _storage.size() == nameSize();
      }
      int colSize = _nodes[0].size();
      int rowSize = _nodes.size();
      for(int i = 0; i < colSize; i++){
         if(_storage.size() == nameSize()) break; //剪枝操作：所有特征都达到特征数量阈值就跳出循环
         const string& featureName = _nodes[0][i]->getFeature();
         for(int j = 0; j < rowSize; j++){
            if(_storage.find(featureName) != _storage.end()) continue; //剪枝操作：如果该特征已达到数量阈值就跳过
            _counter[featureName].insert(_nodes[j][i]->getNum());
            if(_counter[featureName].size() >= _pre[featureName]){
              _storage.insert(featureName);
            }
         }
      }
      if(nameSize() == _storage.size()){
        return true;
      }else{
        //已达到特征数量阈值的特征，组成新的频繁模式
        string prevalentPattern;
        int count = 0;
        for(auto str: _storage){
          prevalentPattern += str;
          count ++;
        }
        if(count >= 2) ans.push_back(prevalentPattern);
        return false;
      }
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
        if(_counter.empty()){
          _counter = counter;
        }else{
          for(auto str: _patterns){
            _counter[str].insert(counter[str].begin(), counter[str].end());
          }
        }
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
//将表实例们按照阶数从大到小排序
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
//主算法程序
void CPPM::algorithm(){
    unique_ptr<Time> t(new Time()); //开始计时的位置，跳过移动这行函数来计算开始计时的位置
    vector< vector<Node*> > maxCliques = bronKerbosch();
    //将极大团加入表实例中
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
        //剪枝操作：检查是否有超集已经加入答案中
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
    cout << "算法花费时间："  << t->getTime() << endl;
    char str[128];
    sprintf(str,"shenZhen with maximal cliques|| dataNum:%d --PI:%f--d:%d-- time:%s", (int)getAllNodes().size(), Graph::getPIPre(), Graph::getDisPre(), to_string(t->getTime()).c_str());
    writeToFile(str,"out.txt");
}