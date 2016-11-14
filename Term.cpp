#include "Term.h"
#include <map>
#include <math.h>

Term::Term(string line) {
    nt = 1;
    term = line;
}

void Term::addInfo(int ndoc, int pos) { // Adds a new position of the term in a specific document


    it = docs.find(ndoc);
    if (it == docs.end()) {
        //cout << "Not in the list. Making new DocInfo!" << endl;
        d = new DocInfo(ndoc);
        d->addNewPos(pos);

        docs.insert(pair<int, DocInfo>(ndoc, *d));
    } else {
        //cout << "This doc is already in the list. Proceed to push back:  " << "Position--> " << pos << " doc--> " << ndoc << endl;
        docs.at(it->first).addNewPos(pos);
        docs.at(it->first).addFreq();
    }

}

void Term::writeToFile(ofstream *out) {
    cout << "Writing----------" << endl;
    for (it = docs.begin(); it != docs.end(); ++it) {

        try {
            // cout << "Writing to file" << endl;
            *out << "doc " << it->first << " ";
            docs.at(it->first).writeToFile(out);
            // cout << "Written...." << endl;
        } catch (exception& e) {
            // cout << "Failed to write: " << e.what() << endl;
        }

    }
    // cout << "For ended..." << endl;
}

void Term::increament() { // Increases nt by 1.
    nt++;
}

string Term::getTerm() { //Returns the term string
    return term;
}

int Term::getMapSize() { //Returns the map size.
    return docs.size();
}

unordered_map<int, DocInfo>* Term::getMapDoc() {
    return &docs;
}

void Term::calcWeight(int i, int mf, int ndoc) {

    //docs.getposSize();
    it = docs.find(i);
    if (it == docs.end()) {
        //cout<<"den brika"<<endl;

    } else {

        int f = it->second.getposSize();

        double TF = double(f) / double(mf);
        double IDF = log2(1 + (float(ndoc) / float(getMapSize())));
        //cout<< "diairesh "<<(float(ndoc) / float(getMapSize()))<<endl;
        double w = IDF*TF;
        it->second.addWeight(w);

//        cout << "STO " << i << endl;
//        cout << "MAXF  " << mf << " F " << f << endl;
//        cout << "N " << ndoc << endl;
//        cout << "Map " << getMapSize() << endl;
//        cout << "TF " << TF << endl;
//        cout << "IDF " << IDF << endl;
//        cout << "W " << it->second.getWeight() << endl;

    }

}

double Term::getw(int i) {

    it = docs.find(i);
    if (it == docs.end()) {
        //	cout<<"den brika"<<endl;
        return 0;
    } else {
        //	cout<<"brika"<<endl;

        return double(it->second.getWeight());

    }
}