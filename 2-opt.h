#ifndef DEUX_OPT
#define DEUX_OPT

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include "backtracking.h"
#include <bits/stdc++.h>

using namespace std;

int* deux_opt1(int N, int* solution, double* matrice){
    int* new_sol = new int[N+1];
    int* best_sol = new int[N+1];
    copy(solution, solution+N+1, best_sol);
    double best_score = valeur_solution(N, solution, matrice);
    bool flag = true;
    while(flag){
        flag = false;
        for(int i=0;i<N;i++){
            for(int j=i+2;j<N;j++){
                copy(best_sol, best_sol+N+1, new_sol);
                for(int k=i+1;k<j+1;k++){
                    new_sol[k]=best_sol[j-k+i+1];
                }
                if(valeur_solution(N, new_sol, matrice) < best_score){
                    best_score = valeur_solution(N, new_sol, matrice);
                    flag = true;
                    copy(new_sol, new_sol+N+1, best_sol);
                    break;
                }
            }
            if(flag) break;
        }
    }
    
    delete new_sol;
    return best_sol;
}

int* deux_opt2(int N, int* solution, double* matrice){
    int* new_sol = new int[N+1];
    int* best_sol = new int[N+1];
    copy(solution, solution+N+1, best_sol);
    double best_score = valeur_solution(N, solution, matrice);
    for(int i=0;i<N;i++){
        for(int j=i+2;j<N;j++){
            copy(best_sol, best_sol+N+1, new_sol);
            for(int k=i+1;k<j+1;k++){
                new_sol[k]=best_sol[j-k+i+1];
            }
            if(valeur_solution(N, new_sol, matrice) < best_score){
                best_score = valeur_solution(N, new_sol, matrice);
                copy(new_sol, new_sol+N+1, best_sol);
            }
        }
    }
    
    delete new_sol;
    return best_sol;
}

int* deux_opt3(int N, int* solution, double* matrice){
    int* new_sol = new int[N+1];
    int* best_sol = new int[N+1];
    copy(solution, solution+N+1, best_sol);
    double best_score = valeur_solution(N, solution, matrice);
    for(int i=0;i<N;i++){
        for(int j=i+2;j<N;j++){
            copy(solution, solution+N+1, new_sol);
            for(int k=i+1;k<j+1;k++){
                new_sol[k]=solution[j-k+i+1];
            }
            if(valeur_solution(N, new_sol, matrice) < best_score){
                best_score = valeur_solution(N, new_sol, matrice);
                copy(new_sol, new_sol+N+1, best_sol);
            }
        }
    }
    
    delete new_sol;
    return best_sol;
}

#endif