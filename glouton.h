#ifndef GLOUTON_H
#define GLOUTON_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include <bits/stdc++.h>

using namespace std;

int calcule_poids(vector<Arete*> &aretes){
    int somme = 0;
    for(Arete* a : aretes){
        somme += a->poids;
    }
    return somme;
}

vector<Arete*>* glouton1(int N, vector<Arete*> &aretes, int x0){//Liste d'arêtes TRIEE
    vector<Arete*>* solution = new vector<Arete*>;
    unordered_set<int> sommet_explores;
    sommet_explores.insert(x0);
    int sommet_actuel = x0;
    while(solution->size()!=N-1){
        for(int i=0;i<aretes.size();i++){
            Arete* a = aretes.at(i);
            if(a->sommet1==sommet_actuel && sommet_explores.find(a->sommet2)==sommet_explores.end()){
                sommet_actuel = a->sommet2;
                sommet_explores.insert(sommet_actuel);
                solution->push_back(a);
                break;
            }
            if(a->sommet2==sommet_actuel && sommet_explores.find(a->sommet1)==sommet_explores.end()){
                sommet_actuel = a->sommet1;
                sommet_explores.insert(sommet_actuel);
                solution->push_back(a);
                break;
            }
        }
    }
    for(int i=0;i<aretes.size();i++){
        Arete* a = aretes.at(i);
        if(a->sommet1==x0 && a->sommet2==sommet_actuel){
            solution->push_back(a);
            return solution;
        }
        if(a->sommet2==x0 && a->sommet1==sommet_actuel){
            solution->push_back(a);
            return solution;
        }
    }
}

vector<Arete*>* glouton2(int N, vector<Arete*> &aretes){
    vector<Arete*>* solution = new vector<Arete*>();
    vector<Arete*>* s = new vector<Arete*>();
    solution =glouton1(N, aretes, 0);
    int score = calcule_poids(*solution);
    for(int x0=1;x0<N;x0++){
        
        s = glouton1(N, aretes, x0);
        if (calcule_poids(*s) < score){
            score = calcule_poids(*s);
            solution = s;
        }
    }
    return solution;
}

#endif