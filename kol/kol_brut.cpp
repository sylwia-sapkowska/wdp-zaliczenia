#include <bits/stdc++.h>
#include "kol.h"
using namespace std;

vector<vector<interesant*>> okienka;
int counter = 0;

void otwarcie_urzedu(int m){
    okienka.resize(m);
}

interesant *nowy_interesant(int k){
    interesant *i = new interesant(counter++);
    okienka[k].push_back(i);
    return i;
}

int numerek(interesant *i){
    return i->id;
}

void print_all(){
    for (int i = 0; i < okienka.size(); i++){
        cout << i << ": ";
        for (auto x: okienka[i]){
            cout << x->id << " ";
        }
        cout << endl;
    }
}

interesant *obsluz(int k){
    if (okienka.empty()) return NULL;
    if (okienka[k].empty()) return NULL;
    interesant *i = okienka[k].front();
    okienka[k].erase(okienka[k].begin());
    // print_all();
    return i;
}

void naczelnik(int k){
    if (okienka[k].empty()) return;
    reverse(okienka[k].begin(), okienka[k].end());
    // print_all();
}

vector<interesant*> fast_track(interesant *i1, interesant *i2){
    vector<interesant*> res;
    for (int i = 0; i < okienka.size(); i++){
        for (int j = 0; j < okienka[i].size(); j++){
            if (okienka[i][j] == i1){
                while (okienka[i][j] != i2){
                    res.push_back(okienka[i][j]);
                    j++;
                }
                res.push_back(i2);
                //erase interval [i1, i2]
                okienka[i].erase(okienka[i].begin() + j - res.size() + 1, okienka[i].begin() + j + 1);
                return res;
            }
        }
    }
    return res;
}

void push_back(interesant *i, int k){
    okienka[k].push_back(i);
}

void zmiana_okienka(interesant *i, int k){
    for (int j = 0; j < okienka.size(); j++){
        for (int l = 0; l < okienka[j].size(); l++){
            if (okienka[j][l] == i){
                okienka[j].erase(okienka[j].begin() + l);
                okienka[k].push_back(i);
                return;
            }
        }
    }
}

void zamkniecie_okienka(int k1, int k2){
    // cerr << "---- " << k1 << " to " << k2 << endl;
    // for (auto x: okienka[k1]){
    //     cerr << x->id << " ";
    // }
    // cerr << endl;
    // for (auto x: okienka[k2]){
    //     cerr << x->id << " ";
    // }
    // cerr << endl;
    if (k1 == k2) return;
    for (int i = 0; i < okienka[k1].size(); i++){
        okienka[k2].push_back(okienka[k1][i]);
    }
    okienka[k1].clear();
}

vector<interesant*> zamkniecie_urzedu(){
    vector<interesant*> res;
    for (int i = 0; i < okienka.size(); i++){
        for (int j = 0; j < okienka[i].size(); j++){
            res.push_back(okienka[i][j]);
        }
    }
    okienka.clear();
    return res;
}