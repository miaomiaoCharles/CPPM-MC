#include "node.hpp"
#include "graph.hpp"
Node::Node(double x, double y, string name):_x(x), _y(y){
    _feature = name[0];
    _num = stoi(name.substr(1));
}

Node::Node(double x, double y, char feature, int number):_x(x), _y(y), _feature(feature), _num(number){}
Node::Node(string name){
    _feature = name[0];
    _num = stoi(name.substr(1));
    _x = 0;
    _y = 0;
}
bool Node::isNeighbor(const Node& node, int disPre){
    return ((this->_x - node._x)*(this->_x - node._x) + (this->_y - node._y)*(this->_y - node._y) <= disPre * disPre && *this < node) ? true: false;
}

bool Node::operator < (const Node& node){
    if(this->_feature != node._feature){
        return this->_feature < node._feature;
    }else{
        return this->_num < node._num;
    }
}
bool Node::operator < (Node* node){
    if(this->_feature != node->_feature){
        return this->_feature < node->_feature;
    }else{
        return this->_num < node->_num;
    }
}
bool Node::operator == (const Node& node){
    if(this->_feature == node._feature && this->_num == node._num){
        return true;
    }else{
        return false;
    }
}
bool Node::operator == (Node* node){
    if(this->_feature == node->_feature && this->_num == node->_num){
        return true;
    }else{
        return false;
    }
    
}

string Node::getName(){
    return this->_feature + to_string(this->_num);
}
