#include "spatioNode.hpp"
int counter = 0;
double Road::_timeSpan = 2;
// SpatioNode::SpatioNode(int travelTime):_travelTime(travelTime){

// }

SpatioNode::SpatioNode(string name){
    int index = 0;
    while(name[index] >= 'A' && name[index] <='Z'){
        index ++;
    }
    _roadName = name.substr(0, index);
    _timeSpan = stoi(name.substr(index, name.size()-index));
}
string SpatioNode::roadName()const{
    return _roadName;
}
string SpatioNode::getInsName()const{
    return _roadName + to_string(_timeSpan);
}
int SpatioNode::getTimeSpan()const{
    return _timeSpan;
}
bool SpatioNode::operator == (const SpatioNode& node)const{
    return this->getInsName() == node.getInsName();
}

bool SpatioNode::operator < (const SpatioNode& node)const{
    return this->getInsName() < node.getInsName();
}

Road::Road(string roadName):_roadName(roadName){
    
}

bool  Road::operator < (const Road& r)const{
    return this->_roadName < r._roadName;
}
bool  Road::operator == (const Road& r)const{
    return this->_roadName == r._roadName;
}

//  void Road::operator = ( Road& r)const{
//     this->_roadName = r._roadName;
//     this->congestionTimes = r.congestionTimes;
//  }

// void Road::addInstance(SpatioNode ins){
//     congestionTimes.push_back(ins);
// }
string Road:: name()const{
    return _roadName;
}

