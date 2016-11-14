#include "DocInfo.h"

DocInfo::DocInfo(int n) { //Constructor
    ndoc = n;
    freq=1;
    weight=1;
}

void DocInfo::addNewPos(int newPos) { //Adding a new position to the list.
    pos.push_back(newPos);
    //cout << "New Position added" << endl;
}
void DocInfo::addFreq(){
	freq++;
}
int DocInfo::getNdoc() { //Returns the doc number.
    return ndoc;
}
int DocInfo::getposSize() { //Returns the doc number.
    //cout<<"SpoS"<<freq<<endl;
	return freq;
}

double DocInfo::getWeight() { //Returns the doc number.
    return weight;
}
void DocInfo::addWeight(double w) { //Returns the doc number.
     weight=w;
}

void DocInfo::writeToFile(ofstream *out) {
    list<int>::iterator it;

    for (it = pos.begin(); it != pos.end(); ++it) {
        *out << *it << " ";
    }
}