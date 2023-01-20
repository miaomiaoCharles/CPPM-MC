#pragma once
#include <iostream>
#include <fstream>
using namespace std;
void writeToFile(string str, string fileName = "runningTime.txt"){
    ofstream fOut;
    fOut.open("../"+fileName,ios::app);
    if (!fOut)
    {
        std::cout << "Open output file faild." << std::endl;
    }
    fOut << str << endl;
    fOut.close();
}