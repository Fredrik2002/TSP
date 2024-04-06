#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>
#include "branch_and_bound.h"
#include "glouton.h"
#include "backtracking.h"
#include "2-approx.h"
#include <bits/stdc++.h>
#include "christofides.h"
#include "branch_and_bound2.h"
#include "held-karp.h"
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
    ofstream instances;
    instances.open("main1/instances.txt", ios::app);
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
            instances << distance(X[i], X[j], Y[i], Y[j]) << " ";
        }
        instances << endl;
    }
    instances << endl;
    instances.close();
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    return aretes;
}

int main(){
    srand(time(NULL));
    /*Les fonctions vont renvoyer des tuples :
        - La liste des arêtes de la solution
        - Le nombre de noeuds explorés
*/

    int i=0;
    int N=15;
    int m = N*(N-1)/2;
    ofstream my_file_approx, my_file_exacte, instances;
    my_file_approx.open("main1/datas_approx.csv");
    my_file_exacte.open("main1/datas_exacte.csv");
    instances.open("main1/instances.txt");
    my_file_approx << "Solution exacte, Solution Gloutonne 1, Solution Gloutonne 2, Solution 2-Approximation, Solution 3/2-Approximation \n";
    my_file_exacte << "Temps de résolution Backtracking, Temps de résolution Branch & Bound1 (Orienté arête),"
    "Nombre de noeuds explorés Branch & Bound1, Temps de résolution Branch & Bound2 (Orienté sommet),"
    "Nombre de noeuds explorés Branch & Bound2, Temps de résolution programmation dynamique, Temps de résolution PLNE \n";
    my_file_approx << N << "\n";
    my_file_exacte << N << "\n";
    Arete* aretes2 = new Arete[m];
    EvalPerf PE;
    while(i<100){
        vector<Arete*> aretes = genere_instances(N, 100, 100, distance_de_manhattan);
        double* matrice = matrice_distance(N, aretes);
        for(int i=0;i<m;i++){
            aretes2[i] = *(aretes.at(i));
        }
        // SOLUTIONS APPROCHEES
        
        double g1 = valeur_solution(N, glouton1(N, matrice, 0), matrice);
        int* solution_gloutonne = glouton2(N, matrice);
        double g2 = valeur_solution(N, solution_gloutonne, matrice);
        double approx1 = deux_approx(N, aretes);
        double approx2 = christofides(N, aretes);

        double best_approx = (g2<approx2) ? g2 : approx2;
        

        //SOLUTIONS EXACTES
        
        PE.start();
        double backtrck = 0;//backtracking(N, aretes);
        PE.stop();
        double d1 = PE.seconds();
        cout << d1 << "s ";
        PE.clear();
        
        PE.start();
        tuple<double, int> couple = lance_profondeur(N, aretes2, best_approx);
        PE.stop();
        double s1 = get<0>(couple);
        int nb_noeuds = get<1>(couple);
        double d2 = PE.seconds();
        cout << d2 << "s, "<<nb_noeuds<<" noeuds ";
        PE.clear();
        

        PE.start();
        tuple<double, int> couple2= lance_profondeur3(N, matrice, best_approx);
        PE.stop();
        int nb_noeuds2 = get<1>(couple2);
        double s2 = get<0>(couple2);
        double d3 = PE.seconds();
        cout <<d3 <<"s, "<<nb_noeuds2<<" noeuds ";
        PE.clear();
        
        

        PE.start();
        vector<vector<int>> state(N);
        for(auto & neighbors : state)
            neighbors = vector<int>((1 << N) - 1, 100000);
        double h_k = held_karp(N, matrice, 0,1, state);
        PE.stop();
        double d4 = PE.seconds();
        cout << " " << d4 <<"s, "<< endl;
        PE.clear();
        if(!(g2<=g1)){
            
            for(int i=0;i<m;i++){
                aretes2[i].afficher();
            }
            cout << "Branch & Bound1 :" << s1 <<endl;
            cout << "backtrck :" << backtrck<<endl;
            cout << "Branch & Bound 2 :" << s2<<endl;
            cout << "g1 :" << g1 << " g2 : " << g2 << endl;
            break;
        }
        else{
            my_file_approx<<backtrck<<"," << g1 <<","<<g2<<","<< approx1<<","<<approx2<<"\n";
            my_file_exacte <<d1 <<","<< d2<< ","<<nb_noeuds<<",";
            my_file_exacte << d3 <<","<<nb_noeuds2<<","<<d4<<",\n";
        }
        i++;
    }
    my_file_approx.close();
    my_file_exacte.close();
    cout << "Exited successfully" << endl;
}