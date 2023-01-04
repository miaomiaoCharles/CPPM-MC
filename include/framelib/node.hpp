#ifndef NODE
#define NODE
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Node{
public:
    Node(double x, double y, string name);
    Node(double x, double y, string featureName, int number);
    Node(string name);
    string getName();
    bool isNeighbor(const Node& node, int disPre);
    bool operator < (const Node& node);
    bool operator < (Node* node);
    bool operator ==(const Node& node);
    bool operator ==(Node* node);
    vector<Node*> _neighbors;
    string getFeature(){return _feature;}
    int  getNum(){return _num;}
    double getX(){return _x;}
    double getY(){return _y;}
private:
    double _x;
    double _y;
    string _feature;
    int _num;
};

#endif