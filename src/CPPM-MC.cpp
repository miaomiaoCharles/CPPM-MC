#include "CPPM-MC.hpp"

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

void CPPM::algorithm(){
    vector< vector<Node*> > maxCliques = getMaximalCliques();
    for(int i = 0; i < maxCliques.size(); i++){
        vector<Node*>& clique = maxCliques[i];
        string str = getPatternFromNodes(clique);
        if(str.size() < 2) continue;
        all_groups[str].addNodes(clique);
    }
}