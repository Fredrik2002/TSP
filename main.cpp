#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <tuple>
#include "algo1.h"
#include "glouton.h"
#include "backtracking.h"
#include "2-approx.h"
#include <bits/stdc++.h>
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
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]));
            aretes.push_back(a);
        }
    }
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    return aretes;
}

void benchmark(vector<tuple<vector<Arete>, int>> liste_fonction){

}

int main(){
    srand(time(NULL));
    /*Les fonctions vont renvoyer des tuples :
        - La liste des arêtes de la solution
        - Le nombre de noeuds explorés
*/
    int N = 10;
    vector<tuple<vector<Arete>, int>> liste_fonction;

    vector<Arete*> aretes = genere_instances(N, 100, 100, distance_de_manhattan);
    

    for(Arete *a : aretes){
        a->afficher();
    }

    vector<Arete*>* solution = new vector<Arete*>();
    solution = glouton1(N, aretes, 0);
    cout <<endl << "Glouton 1 :" << valeur_solution(*solution);

    solution = glouton2(N, aretes);
    cout <<endl << "Glouton 2 :" << valeur_solution(*solution);

    solution = deux_approx(N, aretes);
    cout <<endl << "2-approx :" << valeur_solution(*solution);

    solution = backtracking(N, aretes);
    cout <<endl << "Backtracking :" << valeur_solution(*solution);

    solution = algorithme1(N, aretes);
    cout <<endl << "Algo 1 :" << valeur_solution(*solution);

    
}