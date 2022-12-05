#include "mySPCP.hpp"


set<string> ans;

class Row{
public:
    Row(){};
    Row(vector<Node*> v){
        for(int i = 0; i < v.size(); i++){
            if(_row.find(v[i]->getFeature()) != _row.end()){
                _row[v[i]->getFeature()] ++;
            }else{
                _row.insert({v[i]->getFeature(), 1});
                _pattern.push_back(v[i]->getFeature());
            }
        }
        sort(_pattern.begin(), _pattern.end());
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




int check(int sorceRowLoc, vector<Row>& rows){
    Row& row = rows[sorceRowLoc];
    for(int j = sorceRowLoc-1; j >= 0; j--){
        if(rows[j].getPattern() == row.getPattern()){
            return j;
        }
    }
    return -1;
}

void join(vector<Row>& rows, vector<Row> undo){
    if(undo.size() == 0) return;
    for(auto row: undo){
        if(find(ans.begin(), ans.end(), row.getPattern()) != ans.end()) continue;
        vector<Row> temp = row.porun();
        for(int i = 0; i < temp.size(); i++){
            rows.push_back(temp[i]);
        }
    }
}

bool ifzixulie(string s, string t)  //这个函数用来判断一个str是否是另一个str的子序列
{
	int i=0, j=0;
    int n = t.size();
    int m = s.size();
	while (i < n)
	{
		if (s[j] == t[i])
		{
			i++;
			j++;
			if (i != n&&j == m)      
				return 0;
		}
		else
		{
			j++;
			if (j == m)
				return 0;
		}
	}
	return 1;
}



inline bool checkStr(const set<string>& ans, string str){
    for(auto s: ans){
        if(ifzixulie(s,str)){
            return true;
        }
    }
    return false;
}




void MySPCP::algorithm()
{
    vector<vector<Node *> > maxCliques = getMaximalCliques();
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
    vector<Row> undo;
    for(auto it = all_row.rbegin(); it != all_row.rend(); it++){
        int itSize = it->first;
        vector<Row>& rows = it->second;
        join(rows, undo);  //将之前的不频繁的拆解成k-1阶加入rows
        undo.clear();
        for(int i = 0; i < rows.size(); i++){
            // Row row = it->second[i];
            int fatherLoc = check(i, it->second);
            if(fatherLoc != -1){
                rows[i] += rows[fatherLoc];
            }
            if(rows[i].size()>=2 && !checkStr(ans,rows[i].getPattern()) && rows[i].isPrevalent()){
                ans.insert(rows[i].getPattern());
            }else{
                if(rows[i].size() > 2){
                    undo.push_back(rows[i]);
                }
                
            }
        }
    }
    for(auto a: ans){
        cout << a << endl;
    }
}
