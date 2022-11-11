#include "graph.hpp"
#include "node.hpp"
#include "data.hpp"
map<char, int> Graph::_featureNum;
double Graph::_PIPre = 0;
int Graph::_disPre = 10;
int Graph:: _timeSpan = 2;//时空模式专用
int Graph::_t_threshold = 2;//时空模式专用
Graph::Graph(double pi, int disPre, int maxLength, char *fileName): _maxLength(maxLength)
{
    //以下是对参数进行判断，判断合法性
    _PIPre = pi;
    _disPre = disPre;
    Data input;
    string fileStr = "";
    fileStr += fileName;
    _strData = input(fileStr);
    int model = getModel();
    if (model== 0)
    {
        for (int i = 0; i < _strData.size(); i++)
        {
            double x = stod(_strData[i][2]);
            double y = stod(_strData[i][3]);
            if(x > maxLength || y > maxLength) continue;
            Node node(x, y, _strData[i][1][0], stoi(_strData[i][0])); 
            char fea = node.getFeature();
            if (_featureNum.find(fea) != _featureNum.end())
            {
                _featureNum[fea]++;
            }
            else
            {
                _featureNum[fea] = 1;
            }
            _nodes.push_back(node);
        }
        sort(_nodes.begin(), _nodes.end());
        for (int i = 0; i < _nodes.size() - 1; i++)
        {
            for (int j = i + 1; j < _nodes.size(); j++)
            {
                if (_nodes[i].isNeighbor(_nodes[j], disPre))
                {
                    _nodes[i]._neighbors.push_back(&_nodes[j]);
                }
            }
        }
    }else{
        unordered_map<string, int> m; //记录邻居节点在_node中出现的位置
        for(int i = 0; i < _strData.size(); i++){
            Node SorceNode(_strData[i][0]);
            char fea = SorceNode.getFeature();
            if (_featureNum.find(fea) != _featureNum.end())
            {
                _featureNum[fea]++;
            }
            else
            {
                _featureNum[fea] = 1;
            }
            _nodes.push_back(SorceNode);
            m.insert({SorceNode.getName(), _nodes.size()-1});
        }
        for(int i = 0; i < _strData.size(); i++){
            if(_strData[i].size() >= 2){
                for(int j = 1; j < _strData[i].size(); j++){
                    _nodes[i]._neighbors.push_back(&(_nodes[m[_strData[i][j]]]));
                }
            }
        }
        sort(_nodes.begin(), _nodes.end());
    }
}
Graph::Graph(double pi, double s_threshold, int timeSpan, int t_threshold, char* fileName){
    Data input;  //时空模式的数据只支持一种数据格式
    _timeSpan = timeSpan;
    _PIPre = pi;
    _t_threshold = t_threshold;
    string fileStr = "";
    fileStr += fileName;
    _strData = input(fileStr);
    int i = 0;
    for(i = 0; i < _strData.size(); i++){
        if(_strData[i][0] == "congestionInstance:") break;
        allRoad.push_back(Road(_strData[i][0]));
    }
    for(i = 0; i < _strData.size(); i++){
        if(_strData[i][0] == "congestionInstance:") break;
        if(_strData[i].size() <= 1) continue;
        for(int j = 1; j < _strData[i].size(); j++){
            string name = _strData[i][j];
            for(Road& t:allRoad){
                if(t.name() == name){
                    allRoad[i]._neighbor.push_back(&t);
                }
            }
        }
    }
    //开始接收拥塞实例
    i++;
    for(; i < _strData.size(); i++){
        for(int j = 0; j < _strData[i].size(); j++){
            allInstance.push_back(SpatioNode(_strData[i][j]));
        }
    }
    //将实例放入对应的Road里去
    for(int i = 0; i < allRoad.size(); i++){
        Road& r = allRoad[i];
        for(SpatioNode& ins: allInstance){
            if(ins.roadName() == r.name()){
                r.congestionTimes.push_back(&ins);
                ins.feature = &(allRoad[i]);
            }
        }   
    }
}

bool isclique(vector<Node *> cur, Node *node)
{
    if (cur.size() == 0)
        return true;
    for (auto n : cur)
    {
        if (find(n->_neighbors.begin(), n->_neighbors.end(), node) == n->_neighbors.end())
        {
            return false;
        }
    }
    return true;
}

void disassemble(set<vector<Node *>> &ans, vector<Node *> cur)
{
    ans.insert(cur);
    if (cur.size() == 2)
    {
        return;
    }
    for (int i = 0; i < cur.size(); i++)
    {
        vector<Node *> tempCur = cur;
        tempCur.erase(tempCur.begin() + i);
        disassemble(ans, tempCur);
    }
}

vector<vector<Node *>> Graph::getCliques()
{
    set<vector<Node *>> ansSet;
    vector<vector<Node *>> ans;
    vector<vector<Node *>> maximalCliques = getMaximalCliques();
    for (int i = 0; i < maximalCliques.size(); i++)
    {
        vector<Node *> maxClique = maximalCliques[i];
        disassemble(ansSet, maxClique);
    }
    for (auto it = ansSet.begin(); it != ansSet.end(); it++)
    {
        ans.push_back(*it);
    }
    return ans;
}
int Graph::loadFeatureNum(char c)
{
    return _featureNum[c];
}

bool check(vector<Node*> child, vector<Node*> father){
    for(int i = 0; i < child.size(); i++){
        int j = 0;
        for(j = 0; j < father.size(); j++){
            if(child[i] == father[j]){
                break;
            }
        }
        if(j == father.size()) return false;
    }
    return true;
}

vector<vector<Node *>> Graph::getMaximalCliques()
{
    vector<vector<Node*>> ans;
    for (Node &node : _nodes)
    {
        vector<vector<Node *>> cur;
        for (Node *neibor : node._neighbors)
        {
            int j = 0;
            for(j = 0; j < cur.size(); j++){
                if(isclique(cur[j], neibor)){
                    cur[j].push_back(neibor);
                }
            }
            if(j == cur.size()){  //说明不属于任何一个极大团分支，新建一个分支
                cur.push_back(vector<Node*>{neibor});
                cur[cur.size()-1].push_back(&node);
            }
        }
        for(auto c: cur){
            sort(c.begin(), c.end());
            bool flag = false;
            for(auto v: ans){
                if(check(c,v)){
                    flag = true;
                    break;
                }
            }
            if(flag) continue;
            ans.push_back(c);
        }
    }
    return ans;
}

void Graph::printAns(vector<vector<char>> ans)
{
    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 0; j < ans[i].size(); j++)
        {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }
}
int Graph::getModel()
{
    vector<string> strLine = _strData[0];
    if (strLine[0][0] >= '0' && strLine[0][0] <= '9')
    {
        return 0; // data模式返回0
    }
    else
    {
        return 1; // edge模式返回1
    }
}

bool checkMOEC_condition(Node a, Node b){
    if(a.getFeature() != b.getFeature()){
        if((a.getX() - b.getX())*(a.getX() - b.getX()) + (a.getY() - b.getY())*(a.getY()-b.getY()) <= Graph::getDisPre() * Graph::getDisPre()){
            return true;
        }
    }
    return false;
}

inline string changeVecToStr(vector<Node*> v){
    string ans;
    for(auto node: v){
        ans += node->getName();
    }
    return ans;
}

bool check_moec(vector<vector<Node*> >MOECs, vector<Node*> oec ){
    string str = changeVecToStr(oec);
    sort(str.begin(), str.end());
    for(auto v: MOECs){
        string fatherStr = changeVecToStr(v);
        sort(fatherStr.begin(), fatherStr.end());
        if(fatherStr.find(str) != fatherStr.npos){
            return true;
        }
    }
    return false;
}

vector< vector<Node*> > Graph::getMOEC(){
    vector<Node> allNodes = getAllNodes();
    sort(allNodes.begin(), allNodes.end(), 
        [&](Node a, Node b)->bool{return (a.getX() == b.getX())?a.getY() < b.getY(): a.getX() < b.getX();});
    vector< vector<Node*> > oec;
    for(int i = 0;i < allNodes.size(); i++){
        vector<Node*> temp;
        temp.push_back(&allNodes[i]);
        for(int j = i+1; j < allNodes.size(); j++){
            if(checkMOEC_condition(allNodes[i], allNodes[j])){
                temp.push_back(&allNodes[j]);
            }
        }
        if(!check_moec(oec, temp)){
            oec.push_back(temp);
        }
    }
    return oec;
}

bool Graph::checkPourn(string pattern){
    for(int i = 0; i < result.size(); i++){
        if(result[i].find(pattern) != result[i].npos){
            return true;
        }
    }
    return false;
}
