#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <tuple>
#include "branch_and_bound.h"
#include "glouton.h"
#include "backtracking.h"
#include "2-approx.h"
#include <bits/stdc++.h>
#include "christofides.h"
#include "branch_and_bound2.h"
#include "held-karp.h"
#include "2-opt.h"
#include "EvalPerf.h"
using namespace std;

double distance_de_manhattan(int xi, int xj, int yi, int yj){
    //Retourne la distance de Manhattan entre les points (xi, yi) et (xj, yj)
    return abs(xi-xj)+abs(yi-yj);
}

double distance_euclidienne(int xi, int xj, int yi, int yj){
    //Retourne la distance euclidienne entre les points (xi, yi) et (xj, yj)
    return sqrt(pow((xi-xj), 2)+pow((yi-yj), 2));
}

vector<Arete*> genere_instances(int N, int x_max, int y_max, 
double (*distance)(int, int, int, int)){ 
    int* X = new int[N];
    int* Y = new int[N];
    for(int i=0;i<N;i++){
        
        X[i] = rand()%x_max;
        Y[i] = rand()%y_max;
    }
    vector<Arete*> aretes;
    int p=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(j>i){
                Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]), p);
                aretes.push_back(a);
                p++;
            }
        }
    }
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    delete[] X, Y;
    return aretes;
}

int main(){
    srand(time(NULL));
    EvalPerf PE;
    
    ofstream my_file_approx;

    my_file_approx.open("main2/datas_approx.csv");
    my_file_approx << "N, Solution exacte, Solution Gloutonne 1, Solution Gloutonne 2, Solution 2-Approximation, Solution 3/2-Approximation, Solution 2-OPT \n";

    int liste_des_N[] = {10,20,30,40,50,75,100,150,200,300,400,500,750,1000};
    
    for(int i = 0; i < 14 ;i++){
        int N = liste_des_N[i];
        int m = N*(N-1)/2;
        Noeud2::N = N;
        Noeud2::m = m;
        Arete* aretes2 = new Arete[m];
        for(int j=0;j<100;j++){
            cout << j << endl;
            vector<Arete*> aretes = genere_instances(N, 10000, 10000, distance_de_manhattan);
            double* matrice = matrice_distance(N, aretes);
            Noeud2::distances = matrice;
            for(int k=0;k<m;k++){
                aretes2[k] = *(aretes.at(k));
            }
            // SOLUTIONS APPROCHEES
        double g1 = valeur_solution(N, glouton1(N, matrice, 0), matrice);
        
        int* solution_gloutonne = glouton2(N, matrice);
        double g2 = valeur_solution(N, solution_gloutonne, matrice);

        int* solution_approx1 = deux_approx(N, aretes);
        double approx1 = valeur_solution(N, solution_approx1, matrice);

        int* solution_christofides = christofides(N, aretes);
        double approx2 = valeur_solution(N, solution_christofides, matrice);

        int* best_approx = (g2<approx2) ? solution_gloutonne : solution_christofides;
        

        //int* solution_deux_opt1 = deux_opt1(N, best_approx, matrice);
        int* solution_deux_opt2 = deux_opt2(N, best_approx, matrice);
        int* solution_deux_opt3 = deux_opt3(N, best_approx, matrice);

        double valeur_best_approx = min(valeur_solution(N, solution_deux_opt3, matrice), valeur_solution(N, solution_deux_opt2, matrice));
        //valeur_best_approx = min(valeur_best_approx, valeur_solution(N, solution_deux_opt3, matrice));
        

        my_file_approx<<N<<"," << g1 <<","<<g2<<","<< approx1<<","<<approx2<<","<<valeur_best_approx<<"\n";

        delete[] solution_gloutonne, solution_approx1, 
        solution_christofides, best_approx, solution_deux_opt2,solution_deux_opt3;
            
        }
        delete[] aretes2;
        cout << "Termine pour N=" << N << endl;
    }
    
    my_file_approx.close();
    cout << "Exited successfully" << endl;
}