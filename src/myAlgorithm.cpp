#include "myAlgorithm.hpp"

// bool cmp(vector<Node*> a, vector<Node*> b){
//     if(a.size() > b.size()){
//         return true;
//     }else{
//         return false;
//     }
// }

// inline void sortCliques(vector< vector<Node*> >& cliques){
//     sort(cliques.begin(), cliques.end(), cmp);
// }

// class countMap{
// public:
//     countMap(vector<Node*> nodes){
//         for(auto node: nodes){
//             _insNum.push_back(node->getNum());
//             char feature = node->getFeature();
//             if(_counter.find(feature) != _counter.end()){
//                 _counter[feature] ++;
//             }else{
//                 _counter[feature] = 1;
//             }
//             int num = node->getNum();

//         }
//         _size = _counter.size();
//     }
//     int size(){
//         return _size;
//     }
//     map<char, int> getCounter(){
//         return _counter;
//     }
//     int getId(){
//         return _id;
//     }
//     vector<char> getFeature(){
//         vector<char> res;
//         for(auto p: _counter){
//             res.push_back(p.first);
//         }
//         return res;
//     }
//     void print_countMap(){
//         for(auto p: _counter){
//             cout << p.first << ":" << p.second << "|" ;
//         }
//         cout << endl;
//     }
//     int getFatherId(){return _fatherId;}
//     void setFatherId(int id){_fatherId = id;}
//     void setId(int id){_id = id;}
//     vector<int> getInsNum(){return _insNum;}
//     void addCounter(int num, char c){
//         _counter[c] += num;
//     }
//     static int minPre;
// private:
//     int _size;
//     map<char, int> _counter;
//     int _id;
//     int _fatherId;
//     vector<int> _insNum;
// };

// vector<countMap> allCountMap;   //存储所有的实例计数表
// unordered_map<int, countMap*> idToCountmap;  //将id转化成对应的实例计数表
// unordered_map<char, set<int> > quickLocation; //快速定位表，只有一个这样的表，用来快速查找最近的父实例计数表

// inline int changeIteratorToLoc(map<char, int>::const_iterator it, const map<char, int>& m){
//     int loc = 0;
//     auto tempIt = m.begin();
//     while(tempIt != it){
//         tempIt ++;
//         loc ++;
//     }
//     return loc;
// }

// bool check(int id, int fatherid){
//     const map<char, int> counter = idToCountmap[id]->getCounter();
//     const map<char, int> fatherC = idToCountmap[fatherid]->getCounter();
//     for(auto it = counter.begin(); it != counter.end(); it++){
//         char tempChar = it->first;
//         auto findIt = fatherC.find(tempChar);
//         if(findIt == fatherC.end()){
//             return false;
//         }
//     }
//     return true;
// }

// inline int idLocation(countMap& c){
//     // set<int> res = {1,2,3,4,5,6,7,8,9};
//     // for(auto pair: c->getCounter()){
//     //     char f = pair.first;
//     //     set<int> temp = quickLocation[f];
//     //     set_intersection(temp.begin(), temp.end(),res.begin(), res.end(), res.begin());
//     //     quickLocation[f].insert(c->getId());
//     // }
//     // return *(res.cend());
//     int id = c.getId();
//     for(int i = id-1; i >= 1;i--){
//         if(check(id, i)){
//             return i;
//         }
//     }
//     return 0;
// }

// void MyAlgorithm::algorithm(){
//     unordered_map<char, double>  prevalent_minNum;
//     for(auto it = _featureNum.begin(); it != _featureNum.end(); it++){
//         double pre = it->second * getPI();
//         prevalent_minNum[it->first] = pre;
//     }
//     int idCounter = 1;
//     vector< vector<char> > result;
//     vector<vector<Node*> > cliques = getCliques();
//     // sortCliques(cliques);
//     for(auto& clique: cliques){
//         allCountMap.push_back(countMap(clique));
//     }
//     sort(allCountMap.begin(), allCountMap.end(),[&](countMap a, countMap b)->bool{return a.size() > b.size();});
//     for(countMap& c: allCountMap){
//         c.setId(idCounter);
//         idCounter += 1;
//         idToCountmap[c.getId()] = &c;   //登记id
//     }
//     for(auto& c: allCountMap){
//         int fatherId = idLocation(c);  //获取最近的父特征计数表id，很重要也很难的一步
//         c.setFatherId(fatherId);
//         // c.print_countMap() ;
//         // cout << "  id:" << fatherId << endl;
//     }
//     for(countMap& c: allCountMap){
//         if(c.getFatherId() != 0){        //剪枝操作
//             countMap father = *idToCountmap[c.getFatherId()];
//             auto it = find(result.begin(), result.end(), father.getFeature());
//             if(it != result.end()){
//                 result.push_back(c.getFeature());
//                 continue;
//             }
//         }

//         for(pair<const char, int>& pair: c.getCounter()){
//             if(c.getFatherId() != 0){
//                 countMap* fatherC = idToCountmap[c.getFatherId()]; //将父特征id转成父特征计数表
//                 c.addCounter((fatherC-> getCounter())[pair.first], pair.first) ;   //父特征计数累加上去
//             }
//         }
//         bool flag = false;
//         for(auto pair: c.getCounter()){
//             if(pair.second < prevalent_minNum[pair.first]){
//                 flag = true;
//                 break;
//             }
//         }
//         if(!flag) result.push_back(c.getFeature());
//     }
//      printAns(result);
// }

void MyAlgorithm::init()
{
    vector<vector<Node *>> cliques = getCliques();
    // for(int i = 0; i <cliques.size(); i++){
    //     for(int j = 0; j <cliques[i].size(); j++){
    //         cout <<cliques[i][j]->getName() << " ";
    //     }
    //     cout << endl;
    // }
    
    for (auto clique : cliques)
    {
        all_RowIns.push_back(RowIns(clique));
    }
    sort(all_RowIns.begin(), all_RowIns.end());
    for (int i = 0; i < all_RowIns.size(); i++)
    {
        all_RowIns[i].setId(i + 1);
        RowIns *p = &(all_RowIns[i]);
        idToRowInsMap.insert({i + 1, p});
    }
    //初始化quickLocation, Z的ASCII码是90，A的ASCII码是65
    for (int i = 65; i <= 90; i++)
    {
        quickLocation.insert({char(i), {0}});
    }
}

bool MyAlgorithm::check(string pattern){  
    for(const string& str: ans){
        if(str.find(pattern) != str.npos){
            return false;
        }
    }
    return true;
}

void MyAlgorithm::algorithm()
{
    init();
    //开始遍历每一行
    for (RowIns &row : all_RowIns)
    {   
        if(!check(row.getPattern())) continue;  //剪枝系统
        if(row.size() < 2) continue;
        string pattern = row.getPattern();
        //开始启用哈希表来快读定位父节点，首先要满足第一个条件
        set<int> ansSet = quickLocation[pattern[0]];
        for (int i = 1; i < pattern.size(); i++)
        {
            set<int> temp = quickLocation[pattern[i]];
            set<int> ansTemp;
            set_intersection(ansSet.begin(), ansSet.end(), temp.begin(), temp.end(), inserter(ansTemp, ansTemp.begin()));
            ansSet = ansTemp;
        }
        //搞到交集之后，要将自己的id写入哈希表中
        int id = row.getId();
        for (char c : pattern)
        {
            quickLocation[c].insert(id);
        }
        // //针对父节点候选集开始判断第二个条件，如果找到的话就将实例相加
        if (ansSet.size() > 1)//至少都会有一个{0}
        {   
            vector<int> ansVec; //为了方便从后往前遍历，将候选父节点放入vector中
            for (auto it = ansSet.begin(); it != ansSet.end(); it++)
            {
                ansVec.push_back(*it);
            }
            for (int i = ansVec.size() - 1; i > 0; i--)
            {
                int fatherId = ansVec[i];
                RowIns* fatherp = idToRowInsMap[fatherId];
                if(row|fatherp){
                    //找到了真正的父节点
                    row.add_father(*fatherp);
                    if(row.getPattern() == "BC"){
                        cout << row.printStr() << endl;
                    }
                    break;
                }
            }
        }
        if(row.isPrevalent()){
            ans.insert(row.getPattern());
        }
    }
    for(auto a: ans){
        cout << a << endl;
    }
    
}