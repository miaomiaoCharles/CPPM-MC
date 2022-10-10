#include "rowIns.hpp"

map<char,int> tempQuickAdd;    //在二次筛选是顺便记录子rowins每个node在父rowins的坐标，便于之后快速相加。
RowIns::RowIns(vector<Node *> clique)
{
    _minCounter = INT16_MAX;
    unordered_map<char, vector<int>> m;
    for (int i = 0; i < clique.size(); i++)
    {
        m[clique[i]->getFeature()].push_back(clique[i]->getNum());
    }
    for (auto it = m.begin(); it != m.end(); it++)
    {
        _row.insert(RowNode(it->first, it->second));
        _minCounter = min(_minCounter, int(it->second.size()));
        // _map.insert({temp.getFeature(), temp._diffHash});
    }
}

RowIns::RowIns(){}

void RowIns::refersh_minCounter()
{
    int temp = INT16_MAX;
    for (auto it = _row.begin(); it != _row.end(); it++)
    {
        temp = min(temp, it->getInsSize());
    }
    _minCounter = temp;
}

int RowIns::size() const
{
    return _row.size();
}

bool RowIns::operator<(const RowIns &row) const
{
        return this->size() > row.size();
}

bool RowIns::operator==(const RowIns &r)
{
    return this->_row == r._row;
}

string RowIns::getPattern() const
{
    string str;
    for (const RowNode &node : _row)
    {
        str += node.getFeature();
    }
    return str;
}
bool RowIns::isPrevalent() const
{
    // cout << "候选rowins: " << this->printStr() << endl;
    double pi = 1;
    for (const RowNode &node : _row)
    {
        char c = node.getFeature();
        double pr = node.getInsSize() / double(Graph::loadFeatureNum(c));
        pi = min(pi, pr);
    }
    // cout << "PI: " << pi << endl;
    if (pi >= Graph::getPIPre())
    {
        return true;
        // cout << this->printStr() << endl;
    }
    else
    {
        return false;
    }
    
}

int RowIns::getId() const
{
    return _id;
}
void RowIns::setId(int id)
{
    _id = id;
}
string RowIns::printStr() const
{
    string ans;
    for (auto node : _row)
    {
        ans += node.getFeature();
        ans += "<";
        for (int i = 0; i < node.getInsSize(); i++)
        {
            ans += to_string(node.getIns()[i]);
            ans += ",";
        }
        ans += "> | ";
    }
    return ans;
}

bool RowIns::operator|(RowIns *fatherP)
{
    if (this->size() == fatherP->size())
    { // link模式,父节点与子节点的阶数是相等的
        const vector<RowNode> thisVec = this->getNodeVec();
        const vector<RowNode> fatherVec = fatherP->getNodeVec();
        for (int i = 0; i < this->size(); i++)
        {
            auto diff1 = thisVec[i]._diffHash;
            for (auto ins : fatherVec[i].getIns())
            {
                if (diff1[ins] == true)
                    return false;
            }
        }
        return true;
    }
    else
    { //父节点与子节点的阶数不相等
        const vector<RowNode> fatherVec = fatherP->getNodeVec();
        for (const RowNode &node : this->_row)
        {
            for (int i = 0; i < fatherVec.size(); i++)
            {
                if (node.getFeature() == fatherVec[i].getFeature())
                {
                    tempQuickAdd.insert({fatherVec[i].getFeature(), i});
                    auto diff1 = node._diffHash;
                    for (auto ins : fatherVec[i].getIns())
                    {
                        if (diff1[ins] == true)
                            return false;
                    }
                    break;
                }
            }
        }
        return true;
    }
}
const vector<RowNode> RowIns::getNodeVec()
{
    vector<RowNode> ans;
    for (auto node : _row)
    {
        ans.push_back(node);
    }
    return ans;
}

void RowIns::add_father(RowIns& fatherIns){
    const vector<RowNode> fatherVec = fatherIns.getNodeVec();
    vector<RowNode> thisVec = this->getNodeVec();
    set<RowNode> newSet;
    if(this->size() == fatherIns.size()){
        for(int i = 0; i < fatherVec.size(); i++){
            thisVec[i].addIns(fatherVec[i].getIns());
            newSet.insert(thisVec[i]);
        }
        this->_row = newSet;
    }else{
        for(int i = 0; i < thisVec.size(); i++){
            int loc = tempQuickAdd[thisVec[i].getFeature()];
            thisVec[i].addIns(fatherVec[loc].getIns());
            newSet.insert(thisVec[i]);
        }
        this->_row = newSet;
    }
}
