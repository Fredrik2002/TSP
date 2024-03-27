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
    int N = 14;
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
        double approx1 = deux_approx(N, aretes);
        double approx2 = christofides(N, aretes);

        double best_approx = (g2<approx2) ? g2 : approx2;
        

        //SOLUTIONS EXACTES
        
        auto startTime = high_resolution_clock::now();
        double backtrck = backtracking(N, aretes);
        duration<float> d1 = high_resolution_clock::now()-startTime;
        cout << d1.count() << "s ";

        startTime = high_resolution_clock::now();
        cout << "start" << endl;
        tuple<double, int> couple2 = lance_profondeur3(N, matrice, best_approx);
        cout << "end" << endl;
        duration<float> d3 = high_resolution_clock::now()-startTime;
        int nb_noeuds2 = get<1>(couple2);
        cout <<d3.count() <<"s, "<<nb_noeuds2<<" noeuds ";
        double s2 = get<0>(couple2);
        
        startTime = high_resolution_clock::now();
        cout << "start" << endl;
        tuple<double, int> couple = lance_profondeur(N, aretes2, best_approx);
        cout << "end" << endl;
        duration<float> d2 = high_resolution_clock::now()-startTime;
        double s1 = get<0>(couple);
        int nb_noeuds = get<1>(couple);
        cout << d2.count() << "s, "<<nb_noeuds<<" noeuds ";
        

        
        

        startTime = high_resolution_clock::now();
        vector<vector<int>> state(N);
        for(auto & neighbors : state)
            neighbors = vector<int>((1 << N) - 1, 100000);
        double h_k = held_karp(N, matrice, 0,1, state);
        duration<float> d4 = high_resolution_clock::now()-startTime;
        cout << " " << d4.count() <<"s, "<< endl;
    }
    
    // Close the file
    fin.close();


}