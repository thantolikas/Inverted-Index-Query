#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class DocInfo {
private:
    list<int> pos; //Position List
    int ndoc; // Document number/id
    int freq;
    double weight;
public:

    DocInfo(int n);
    int getposSize();
    void addNewPos(int newPos);
    int getNdoc();
    void writeToFile(ofstream *out);
    double getWeight();
    void addWeight(double w);
    void addFreq();

};