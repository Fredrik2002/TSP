#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <bits/stdc++.h>

#include "christofides.h"
#include "backtracking.h"
#include "branch_and_bound2.h"
#include "glouton.h"
#include "held-karp.h"
#include "2-opt.h"
#include "EvalPerf.h"



using namespace std;
using namespace std::chrono;

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
    /*
    Entrée :
        - N : nombres de sommets
        - x_max, y_max : Coordonnées maximales des points que l'on génère
        - *distance : fonction qui mesure la distance entre 2 points
    Sortie :
        - Liste d'arêtes entre tous les sommets avec leurs poids 
    A noter que les sommets sont numérotés de 0 à N-1.
    */
    int* X = new int[N];
    int* Y = new int[N];
    for(int i=0;i<N;i++){
        
        X[i] = rand()%x_max;
        Y[i] = rand()%y_max;
    }
    vector<Arete*> aretes;
    int p=0;
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]), p);
            aretes.push_back(a);
            p++;
        }
    }
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    return aretes;
}

int main(){
    srand(time(NULL));
    ifstream fin;

    string line, str;
    fin.open("test.txt");
    int i=0;
    double* H = new double[3];
    vector<Arete*> aretes;

    while (getline(fin, line)) {
        stringstream ss(line);
        while(getline(ss, str, ',')){
            H[i%3]=stod(str);
            i++;
        }
        aretes.push_back(new Arete((int) H[0], (int) H[1], H[2], i/3-1)); 
    }
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    int N = 20;
    int m = N*(N-1)/2;
    EvalPerf PE;

    aretes = genere_instances(N, 10000, 10000, distance_de_manhattan);
    Arete* aretes2 = new Arete[m];
    int x0 = 0;
    for(int i=0;i<m;i++){
        aretes2[i] = *aretes.at(i);
        aretes2[i].afficher();
    }
    
    double* matrice = matrice_distance(N, aretes);
    // SOLUTIONS APPROCHEES
    double g1 = valeur_solution(N, glouton1(N, matrice, 0), matrice);
    
    int* solution_gloutonne = glouton2(N, matrice);
    double g2 = valeur_solution(N, solution_gloutonne, matrice);

    int* solution_approx1 = deux_approx(N, aretes);
    double approx1 = valeur_solution(N, solution_approx1, matrice);

    int* solution_christofides = christofides(N, aretes);
    double approx2 = valeur_solution(N, solution_christofides, matrice);

    int* best_approx = (g2<approx2) ? solution_gloutonne : solution_christofides;
    

    int* solution_deux_opt1 = deux_opt1(N, best_approx, matrice);
    int* solution_deux_opt2 = deux_opt2(N, best_approx, matrice);
    int* solution_deux_opt3 = deux_opt3(N, best_approx, matrice);

    double valeur_best_approx = min(valeur_solution(N, solution_deux_opt1, matrice), valeur_solution(N, solution_deux_opt2, matrice));
    valeur_best_approx = min(valeur_best_approx, valeur_solution(N, solution_deux_opt3, matrice));
        

        
    
    // Close the file
    fin.close();
}