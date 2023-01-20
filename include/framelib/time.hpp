#pragma once
#include <iostream>
#include<chrono>
using namespace std;

class Time{
public:
    Time(){
        startTime = chrono::system_clock::now();
    }
    int getTime(){
        endTime = chrono::system_clock::now();
        return chrono::duration<double,milli>(endTime - startTime).count();
    }
private:
    chrono::system_clock::time_point startTime;
    chrono::system_clock::time_point endTime;
};


