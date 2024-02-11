#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "algo1.h"
#include <bits/stdc++.h>

using namespace std;

double valeur_solution(vector<Arete*> &solution){
    double somme = 0;
    for(Arete *a : solution){
        somme += a->poids;
    }
    return somme;
}

void placer(int N, double* distances, vector<int> &solution_actuelle, double score_actuel, 
vector<int>* &meilleure_solution, double &meilleur_score, unordered_set<int> &sommet_explores){
    if(solution_actuelle.size()==N){
        if(score_actuel+distances[solution_actuelle.at(0)*N+solution_actuelle.at(N-1)]<meilleur_score){
            meilleure_solution = new vector<int>(solution_actuelle.begin(), solution_actuelle.end());
            meilleur_score = score_actuel+distances[solution_actuelle.at(0)*N+solution_actuelle.at(N-1)];
        }
    }
    else{
        int dernier_sommet = solution_actuelle.at(solution_actuelle.size()-1);
        for(int i=0;i<N;i++){
            if(sommet_explores.find(i)==sommet_explores.end()){ // Si on a pas encore exploré le sommet i
                if(score_actuel+distances[i*N+dernier_sommet]<meilleur_score){// Et que la solution est intéressante
                    solution_actuelle.push_back(i);
                    sommet_explores.insert(i); 
                    placer(N, distances, solution_actuelle, score_actuel+distances[i*N+dernier_sommet], meilleure_solution, meilleur_score, sommet_explores);
                    sommet_explores.erase(i);
                    solution_actuelle.pop_back();
                }
            }
        }
    }
}

vector<Arete*>* reconstruit_solution(int N, vector<Arete*> &aretes, vector<int> solution){
    vector<Arete*>* return_sol = new vector<Arete*>();
    for(int i=1;i<N;i++){
        int b = solution.at(i);
        int c = solution.at(i-1);
        for(Arete *a : aretes){
            if(a->sommet1==b && a->sommet2==c || a->sommet2==b && a->sommet1==c){
                return_sol->push_back(a);
                break;
            }
        }
    }
    int b = solution.at(0);
    int c = solution.at(N-1);
    for(Arete *a : aretes){
        if(a->sommet1==0 && a->sommet2==c || a->sommet2==b && a->sommet1==c){
            return_sol->push_back(a);
            break;
        }
    }
    return return_sol;
}

vector<Arete*>* backtracking(int N, vector<Arete*> &aretes){
    double* distances = new double[N*N];
    for(Arete *a : aretes){
        int i=a->sommet1*N+a->sommet2;
        int j=a->sommet2*N+a->sommet1;
        distances[i]=a->poids;
        distances[j]=a->poids;
    }
    vector<int>* solution_finale = new vector<int>();
    unordered_set<int> sommet_explores;
    vector<int> solution_actuelle;
    double meilleur_score = 1000000000;
    double score_actuel = 0;
    solution_actuelle.push_back(0);
    sommet_explores.insert(0);
    placer(N, distances, solution_actuelle, score_actuel, solution_finale, meilleur_score, sommet_explores);
    //Peu importe le sommet de départ
    vector<Arete*>* s = reconstruit_solution(N, aretes, *solution_finale);
    return s;
}

#endif