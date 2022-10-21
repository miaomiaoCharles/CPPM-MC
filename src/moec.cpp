#include "moec.hpp"

// vector<string> porun(string pattern){
//     vector<string> ans;
//     for(char missAlpha: pattern){
//         string temp;
//         for(auto c: pattern){
//             if(c == missAlpha) continue;
//             temp.push_back(c);
//         }
//         ans.push_back(temp);
//     }
//     return ans;
// }

void Moec::algorithm(){
    vector<string> ans;
    vector< vector<Node*> > moec = getMOEC();
    map<char, set<int> >idList;
    for(auto it = _featureNum.begin(); it != _featureNum.end(); it++){
        idList.insert({it->first,{}});
    }
    //生成idset
    map < int, vector<Node*> >  idToClique;
    for(int i = 0; i < moec.size(); i++){
        int id = i+1;
        idToClique.insert({id, moec[i]});
        for(int j = 0; j < moec[i].size(); j++){
            idList[moec[i][j]->getFeature()].insert(id);
        }
    }
    //获取要核查的极大模式
    
    vector<string> patterns;
    sort(moec.begin(), moec.end(), [&](vector<Node*>a, vector<Node*>b)->bool{return a.size() < b.size();});
    for(int i = 0; i < moec.size(); i++){
        string pattern = "";
        for(int j = 0; j < moec[i].size(); j++){
            pattern.push_back(moec[i][j]->getFeature());
        }
        //对pattern利用set进行查重，去除重复的特征
        set<char> temp;
        for(char c: pattern){
            temp.insert(c);
        }
        pattern.clear();
        for(char c: temp){
            pattern.push_back(c);
        }
        patterns.push_back(pattern);
    }

    //对每一个候选模式开始进行核查
    //首先要将他们的idset取交集
    sort(patterns.begin(), patterns.end(), [&](string a, string b)->bool{return a.size() < b.size();});
    for(int k = 0; k < patterns.size(); k++){
        string pattern = patterns[k];
        set<int> ansSet = idList[pattern[0]];
        for (int i = 1; i < pattern.size(); i++)
        {
            set<int> temp = idList[pattern[i]];
            set<int> ansTemp;
            set_intersection(ansSet.begin(), ansSet.end(), temp.begin(), temp.end(), inserter(ansTemp, ansTemp.begin()));
            ansSet = ansTemp;
        }
        map <char, int> counter;
        for(auto id: ansSet){
            vector < Node*>  moec = idToClique[id];
            for(Node* node: moec){
                char feature = node->getFeature();
                if(pattern.find(feature) != pattern.npos){
                    auto it = counter.find(feature);
                    if(it == counter.end()){
                        counter.insert({feature, 1});
                    }else{
                        counter[feature] ++;
                    }
                }
            }
        }
        double pi = 1000.00;
        for(auto pair: counter){
            double pr = pair.second/double(_featureNum[pair.first]);
            pi = min(pi, pr);
        }
        if(pi > Graph::getPIPre()){
            ans.push_back(pattern);
        }
    }
    //对ans进行去重处理，并且去掉阶数小于2的
    set <string> ansSet;
    for(auto a: ans){
        if(a.size() < 2) continue;
        ansSet.insert(a);
    }
    for(string a: ansSet){
        cout << a << endl;
    }
}