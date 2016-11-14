#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include "Term.h"
#include <sys/time.h>
#include <fstream>
#include <math.h>

using namespace std;

mutex m, m2;
unordered_map<string, Term*> finalMap;
ifstream myfile("input3.txt");
int kop;
int qsize;
int ndoc;
int qid;
int *tp;
double *wp;
int *p;
double *ds;
map<float, int> si;

bool read_line(string *line) {

    {
        lock_guard<mutex> lg(m);
        if (getline(myfile, *line)) {
            return true;
        } else {
            return false;
        }

    }

}

void calc(int i, map<string, int> *query, double *w, int *p) {
    map<string, int>::iterator qt;
    unordered_map<string, Term*>::iterator it;
    for (qt = query->begin(); qt != query->end(); qt++) {

        string str = qt->first;
        //cout<<"STR"<<str<<endl;
        it = finalMap.find(str);
        //cout << "\n" << "Term: " << it->first << endl;
        qsize++;
        //cout<<"DINW i "<<i<<"    p[i-1] "<<p[i-1]<<endl;
        it->second->calcWeight(i, p[i - 1], ndoc);
        w[i] = double(it->second->getw(i) + w[i]);
    }

}

int readquery(int *line_count, map<string, int> *querys, string lines, int *top) {
    int qids;

    map<string, int>::iterator qts;
    //cout<<"LE "<<line<<endl;
    if (*line_count == 0) {
        qids = atoi(lines.c_str());
        //cout << "qid " << qids << endl;
    } else if (*line_count == 1) {
        top[kop] = atoi(lines.c_str());
    } else {
        qts = querys->find(lines);
        if (qts == querys->end()) {
            querys->insert(pair<string, int>(lines, qids));
            //cout<<"kanw insert "<<lines<<endl;
        }
    }
    *line_count = *line_count + 1;
    //cout<<*line_count<<endl;
    return qids;
}

float cquery(string line, double *w, int i) {
    map<string, int> query;
    map<string, int>::iterator qt;
    //cout << "=======================" << endl;
    //cout << "Olokliro line  " << line << endl;
    stringstream s(line);
    int count = 0;
    while (getline(s, line, ' ')) {
        ////NEW
        qid = readquery(&count, &query, line, tp);
    }
    w[i] = 0;
    double *pw;
    pw = w;
    qsize = 0;
    calc(i, &query, pw, p);
   // cout << "oliko baros" << w[i] << endl;
   // cout << "oliko size" << ds[i - 1] << endl;
    //cout << "oliko qsize" << qsize << endl;
    float tr = (sqrt(ds[i - 1]) * sqrt(qsize));
    //cout<<tr<<endl;

    float similarity = float(w[i]) / tr;
    //cout<<similarity<<endl;
    return similarity;


}

auto thread_func = [](int *p, double *dsize) -> void {

    string line;
    unordered_map<string, Term*>::iterator it;

    map<string, int>::iterator ft;
    int doc;
    string word;
    int line_count;
    int k = 0;

    while (read_line(&line)) {
        map<string, int> freq;
        line_count = 0;
        p[k] = 1;
        //cout << "========================================" << endl;
        //cout << "Whole line ---> \"" << line << "\"" << endl;

        istringstream s(line);
        s>>word;
        doc = atoi(line.c_str());
       // cout << "Number of doc: " << doc << endl;

        while ((s >> word)) {
            m2.lock();
            it = finalMap.find(word);
            if (it == finalMap.end()) {
                Term* t = new Term(word);
                finalMap.insert(pair<string, Term*>(word, t));
                t->addInfo(doc, line_count);
            } else {
                // cout << "Word exists in map: " << line << endl;
                finalMap.at(it->first)->increament();
                finalMap.at(it->first)->addInfo(doc, line_count);
                //cout << "Map Size:" << mymap->at(it->first)->getMapSize() << endl;
            }
            m2.unlock();

            ft = freq.find(word);
            if (ft == freq.end()) {
                line_count++;

                freq.insert(pair<string, int>(word, 1));
               // cout << "inserting " << word << " value " << 1 << endl;

            } else {

                ft->second++;
                //cout << "else " << ft->first << " Second " << ft->second << endl;
                if (p[k] <= ft->second)
                    p[k] = ft->second;

            }
        }
        dsize[k] = line_count;


        //cout << "MAX re" << p[k] << "  Count " << k << endl;
        k++;

    }


};

int main() {
    string line;
    timeval tim;
    double t1, t2;
    unordered_map<string, Term*>::iterator it;

    int num_threads = 1;

    vector<thread> workers;

    if (myfile.is_open()) {

        getline(myfile, line);
        ndoc = atoi(line.c_str());
        int maxx[ndoc];
        double dsize[ndoc];
        p = maxx;
        ds = dsize;

        cout << "Starting threads..." << endl;

        for (int i = 0; i < num_threads; i++) {
            workers.push_back(thread(thread_func, p, ds));
        }

        gettimeofday(&tim, NULL);
        t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
        cout << "Waiting for threads to finish..." << endl;
        for (thread& t : workers) {
            t.join();
        }
        cout << "Threads finished..." << endl;
        gettimeofday(&tim, NULL);
        t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
        myfile.close();



        cout << "Inverted Index creation completed!" << endl;
        cout << "Execution time: " << t2 - t1 << " seconds" << endl;
        //for (it = finalMap.begin(); it != finalMap.end(); ++it)
        //   cout << "\n" << "Term: " << it->first << endl;

        //        ofstream* out = new ofstream("inverted5.txt");
        //    
        //        if (out->is_open()) {
        //            for (it = finalMap.begin(); it != finalMap.end(); ++it) {
        //                *out << it->first << " ";
        //                try {
        //                    cout << "Calling writeToFile........" << endl;
        //                    it->second->writeToFile(out);
        //                    cout << "WriteToFile returned..." << endl;
        //                } catch (exception& e) {
        //                    cout << "Failed to call writeToFile: " << e.what() << endl;
        //                }
        //                *out << endl;
        //            }
        //    
        //        } else {
        //            cout << "Can't open output file!" << endl;
        //        }
        //        out->close();
        //        
        //        system("PAUSE");

        double w[ndoc];
        int topk[ndoc];
        float finalarray[3][ndoc];

        tp = topk;
        wp = w;
 
        cout<<endl<<endl<<"Executing queries..."<<endl;
        t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
        ifstream qfile("query.txt");
        if (qfile.is_open()) {
            int queries;

            int ans;
            int qid;
            if (getline(qfile, line)) {
                queries = atoi(line.c_str());
            }
            //cout << " queries " << queries << endl;

            for (int i = 1; i <= ndoc; i++) {
                qfile.clear();
                qfile.seekg(0, ios::beg);
                int count = 0;
               // cout << "document " << i << endl;
                kop = 0;
                if (getline(qfile, line)) {
                }
                map<int, float> si;
                while (getline(qfile, line)) {

                    float similarity = cquery(line, wp, i);
                    //cout << similarity << endl;
                    finalarray[kop][i - 1] = similarity;

                    kop++;
                }
            }
            t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
            cout<<"Queries finished..."<<endl;
            cout<<"Execution time: "<<t2-t1<<endl;
            cout<<"Results: "<<endl;
            for (int i = 0; i < 3; i++) {
                cout<<"Document: "<<i+1<<" returned for queries: ";
                for (int k = 0; k < topk[i]; k++) {
                    float max = 0;
                    int pos = -1;
                    for (int j = 0; j < 3; j++) {
                        if (finalarray[i][j] > max) {
                            max = finalarray[i][j];
                            pos = j;
                        }
                    }
                    
                    //cout << endl << "MAXXX " << finalarray[i][pos] << endl;
                    cout<<pos+2<<" ,";
                    finalarray[i][pos] = 0;

                }
                //cout << "Epomenooooooooooooooooooooo" << endl << endl;
                cout<<endl;
            }
        } else {
            cout << "error" << endl;
        }
        qfile.close();

    } else cout << "Unable to open file";



    //Write inverted to file


}
