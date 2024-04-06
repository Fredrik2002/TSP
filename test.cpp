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
    int N = 10;
    int m = N*(N-1)/2;

    for(int i=0;i<1;i++){
        Arete* aretes2 = new Arete[m];
        int x0 = 0;
        for(int i=0;i<m;i++){
            aretes2[i] = *aretes.at(i);
        }
        double* matrice = matrice_distance(N, aretes);
        
        // SOLUTIONS APPROCHEES
        double g1 = glouton1(N, aretes, 0);
        double g2 = glouton2(N, aretes);
        double g3 = valeur_solution(N, glouton1_2(N, matrice, 0), matrice);
        double approx1 = deux_approx(N, aretes);
        double approx2 = christofides(N, aretes);

        double best_approx = (g2<approx2) ? g2 : approx2;
        int* solution = glouton1(N, matrice, 0);
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                cout << matrice[i*N+j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        for(int i=0;i<N+1;i++){
            cout << solution[i] << " ";
        }
        cout << endl;
        cout << "g1 :" << g1 << " g3 : " << g3 << endl;
        
       
    }
    
    // Close the file
    fin.close();


}