#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "algo1.h"
using namespace std;

double distance_de_manhattan(int xi, int xj, int yi, int yj){
    //Retourne la distance de Manhattan entre les points (xi, yi) et (xj, yj)
    return abs(xi-xj)+abs(yi-yj);
}

double distance_euclidienne(int xi, int xj, int yi, int yj){
    //Retourne la distance euclidienne entre les points (xi, yi) et (xj, yj)
    return sqrt(pow((xi-xj), 2)+pow((yi-yj), 2));
}

vector<Arete> genere_instances(int N, int x_max, int y_max, 
double (*distance)(int xi, int xj, int yi, int yj)){ 
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
        cout << X[i] << Y[i] << endl;
    }
    vector<Arete> aretes;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            Arete a(i, j, distance(X[i], X[j], Y[i], Y[j]));
            aretes.push_back(a);
        }
    }
    return aretes;

}

int main(){
    srand(time(NULL));
    vector<Arete> aretes  = genere_instances(6, 100, 100, distance_de_manhattan);
    for(Arete a : aretes){
        a.afficher();
    }
}