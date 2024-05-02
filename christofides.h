#ifndef CHRISTOFIDES_H
#define CHRISTOFIDES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "arete.h"
#include "2-approx.h"
#include <bits/stdc++.h>
#include "backtracking.h"
#include "library/Graph.cpp"
#include "library/Matching.cpp"
#include "library/BinaryHeap.cpp"
#include "library/Example.cpp"

using namespace std;

vector<int>* sommet_impairs(int N, vector<Arete*> &ACPM){
    int* degres = new int[N]();
    vector<int>* sommets = new vector<int>();
    for(Arete* a : ACPM){
        degres[a->sommet1] += 1;
        degres[a->sommet2] += 1;
    }
    for(int i=0;i<N;i++){
        if(degres[i]%2==1) sommets->push_back(i);
    }
    delete degres;
    return sommets;
}

vector<Arete*> * couplage_parfait(int N, vector<int> &sommets, double* distances){
    ofstream my_file;
    my_file.open("input.txt");
    my_file << sommets.size() << "\n";
    my_file << sommets.size()*(sommets.size()-1)/2 << "\n";
    for(int i=0;i<sommets.size();i++){
        for(int j=i+1;j<sommets.size();j++){
            my_file << i << " " << j << " " << distances[sommets.at(i)*N+sommets.at(j)] << "\n";
        }
    }
    my_file.close();
    vector<int>* matching = MinimumCostPerfectMatchingExample("input.txt");
    vector<Arete*> * aretes_du_matching = new vector<Arete*>();
    for(int i=0;i<matching->size();i+=2){
        aretes_du_matching->push_back(new Arete(sommets.at(matching->at(i)), 
        sommets.at(matching->at(i+1)), distances[sommets.at(matching->at(i))*N+sommets.at(matching->at(i+1))], 1));
    }
    return aretes_du_matching; 
}

int* christofides(int N, vector<Arete*> &aretes, double* distances){
    vector<Arete*> ACPM = kruskal(N+1, aretes, -1);
    vector<int>* sommets = sommet_impairs(N, ACPM);
    vector<Arete*>* matching = couplage_parfait(N, *sommets, distances);
    for(Arete* a: *matching){
        ACPM.push_back(a);
    }
    vector<int> circ = circuit_eulerien(ACPM);
    shortcut(N, circ);
    int* sol = new int[N+1];
    for(int i=0;i<N+1;i++){
        sol[i] = circ.at(i);
    }
    delete sommets, matching;
    return sol;
}

#endif