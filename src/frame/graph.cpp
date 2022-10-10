#include "graph.hpp"
#include "node.hpp"
#include "data.hpp"
map<char, int> Graph::_featureNum;
double Graph::_PIPre = 0;
Graph::Graph(double pi, int disPre, int maxLength, char *fileName) : _disPre(disPre), _maxLength(maxLength)
{
    //以下是对参数进行判断，判断合法性
    _PIPre = pi;
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

vector<vector<Node *>> Graph::getMaximalCliques()
{
    vector<vector<Node *>> ans;
    vector<string> ansStr;
    for (Node &node : _nodes)
    {
        bool flag = false;
        int maxSize = 0;
        vector<Node *> cur;
        cur.push_back(&node);
        string str = "";
        for (Node *neibor : node._neighbors)
        {
            if (isclique(cur, neibor))
            {
                cur.push_back(neibor);
                str += neibor->getName();
            }
        }
        for (int i = 0; i < ansStr.size(); i++)
        {
            if (ansStr[i].find(str) != ansStr[i].npos)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            ans.push_back(cur);
            ansStr.push_back(str);
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