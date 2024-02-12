#ifndef DEUX_APPROX
#define DEUX_APPROX

#include <iostream>
#include <vector>
#include <algorithm>
#include "algo1.h"
#include <bits/stdc++.h>

using namespace std;

vector<Arete*>* deux_approx(int N, vector<Arete*> &aretes){
    vector<Arete*> * solution = new vector<Arete*>();
    vector<Arete*> ACPM = kruskal(N+1, aretes, -1);
    for(Arete* a : ACPM){
        solution->push_back(a);
        solution->push_back(a);
    }
    return solution;
}

#endif