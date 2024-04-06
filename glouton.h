#ifndef GLOUTON_H
#define GLOUTON_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include "backtracking.h"
#include <bits/stdc++.h>

using namespace std;

int* glouton1(int N, double* distances, int x0){
    int sommet_actuel = 0;
    int* solution = new int[N+1];
    int* sommets_visites = new int[N]();
    sommets_visites[x0] = 1;
    solution[N] = x0;
    solution[0] = x0;
    for(int i = 1; i <N; i++){
        double min_dist =100000000;
        int min_sommet = -1;
        for(int j=0; j <N; j++){
            if(sommet_actuel!=j && sommets_visites[j]==0 && distances[sommet_actuel*N+j]<min_dist){
                min_dist = distances[sommet_actuel*N+j];
                min_sommet = j;
            }
        }
        sommets_visites[min_sommet] = 1;
        solution[i] = min_sommet;
        sommet_actuel = min_sommet;
    }
    delete sommets_visites;
    return solution;


}

int* glouton2(int N, double* matrice){
    int* s_finale = glouton1(N, matrice, 0);
    double valeur_sol = valeur_solution(N, s_finale, matrice);
    for(int x0=1;x0<N;x0++){
        int* s = glouton1(N, matrice, x0);
        if (valeur_solution(N, s, matrice) < valeur_sol){
            s_finale = s;
            valeur_sol = valeur_solution(N, s, matrice);
        }
    }
    return s_finale;
}

#endif