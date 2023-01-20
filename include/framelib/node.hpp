#ifndef NODE
#define NODE
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class TreeNode;
class Node{
public:
    Node(double x, double y, string name);
    Node(double x, double y, string featureName, int number);
    Node(string name);
    Node() = default;
    string getName();
    bool isNeighbor(const Node& node, int disPre);
    // bool isNei(Node* node);
    bool operator < (const Node& node);
    bool operator < (Node* node);
    bool operator ==(const Node& node);
    bool operator ==(Node* node);
    vector<Node*> _neighbors;
    string getFeature(){return _feature;}
    int  getNum(){return _num;}
    double getX(){return _x;}
    double getY(){return _y;}
    int _id;
    double _x;
    double _y;
private:
    string _feature;
    int _num;
};

#endif