//主要是一个函数对象，会判断是哪种模式的数据，并将数据以字符串数组的形式返回。
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
class Data
{
public:
    vector< vector<string>>  operator()(string dataName)
    {   
        vector< vector<string>> res;
        ifstream inFile;
        string str;
        inFile.open("../data/" + dataName);
        while (getline(inFile, str))
        {
            str.erase(0, str.find_first_not_of(" "));
            vector<string>  v;
            istringstream ss(str);
            string word;
            while(ss >> word){
                v.push_back(word);
                word.clear();
            }
            res.push_back(v);
        }
        return res;
    }
private:
};