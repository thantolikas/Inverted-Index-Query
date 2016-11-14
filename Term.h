#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "DocInfo.h"

using namespace std;

class Term {
private:
    string term; // The term string
    unordered_map<int, DocInfo>::iterator it; // Just an iterator
    int nt; // Number of appearances in the documents
    DocInfo* d;
public:
    unordered_map<int, DocInfo> docs; // Map in which the key is the number of the doc and the value a DocInfo object
    
    Term(string line);
    void addInfo(int ndoc, int pos) ;
    void writeToFile(ofstream *out) ;
    void increament();
    string getTerm();
    int getMapSize();
    double getw(int i) ;
     void calcWeight(int i,int mf,int ndoc);
    unordered_map<int, DocInfo>* getMapDoc();

};