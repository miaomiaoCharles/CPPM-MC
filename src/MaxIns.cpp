#include "MaxIns.hpp"
#include "node.hpp"
#include "graph.hpp"
#include <stack>
#include <vector>
#include <set>

using namespace std;

class TreeNode{
public:
    TreeNode(Node node){
        _sourceNode = node;
    }
    TreeNode(){
        Node node("root");
        _sourceNode = node;
    }
    vector<TreeNode> _childs;
    bool isNeighbor(TreeNode tnode){
        Node node1 = tnode.getSourceNode();
        Node node2 = this->getSourceNode();
        return ((node1._x - node2._x)*(node1._x - node2._x) + (node1._y - node2._y)*(node1._y - node2._y) <= Graph::getDisPre() * Graph::getDisPre());
    }
    Node getSourceNode(){
        return _sourceNode;
    }
    bool operator <(TreeNode node){
        return this->getSourceNode() < node.getSourceNode();
    }
private:
    Node _sourceNode;
};

void MaxIns::algorithm(){
    stack<TreeNode> st;
    auto allNodes = getAllNodes();
    sort(allNodes.begin(), allNodes.end());
    reverse(allNodes.begin(), allNodes.end());
    for(auto node: getAllNodes()){
        TreeNode tnode(node);
        st.push(tnode);
    }
    TreeNode root;

    while(!st.empty()){
        TreeNode tNode = st.top();
        st.pop();
        stack<TreeNode> tempSt = st;
        vector<TreeNode> maximalIns;
        maximalIns.push_back(tNode);
        while(!tempSt.empty()){
            TreeNode neiNode = tempSt.top();
            tempSt.pop();
            if(tNode.isNeighbor(neiNode)){
                bool flag = true;
                for(auto it = maximalIns.begin(); it != maximalIns.end(); it++){
                    if(!neiNode.isNeighbor(*it)){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    tNode._childs.push_back(neiNode);
                    maximalIns.push_back(neiNode);
                }
            }
        }
    }
}