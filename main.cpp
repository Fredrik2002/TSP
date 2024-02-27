#include <iostream>
#include <fstream>
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
#include "christofides.h"
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
    ofstream instances;
    instances.open("instances.txt", ios::app);
    int* X = new int[N];
    int* Y = new int[N];
    for(int i=0;i<N;i++){
        
        X[i] = rand()%x_max;
        Y[i] = rand()%y_max;
    }
    vector<Arete*> aretes;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(j>i){
                Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]));
                aretes.push_back(a);
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

    vector<Arete*>* g1 = new vector<Arete*>();
    vector<Arete*>* g2 = new vector<Arete*>();
    vector<Arete*>* approx1 = new vector<Arete*>();
    vector<Arete*>* approx2 = new vector<Arete*>();
    vector<Arete*>* bab = new vector<Arete*>();
    vector<Arete*>* backtrck = new vector<Arete*>();

    tuple<vector<Arete*>*, int> couple;
    int i=0;
    int N = 12;
    int nb_noeuds;
    ofstream my_file, instances;
    clock_t startTime;
    double t1, t2;
    my_file.open("datas.csv");
    instances.open("instances.txt");
    my_file << "Solution Gloutonne 1, Solution Gloutonne 2, Solution 2-Approximation, Solution 3/2-Approximation," 
    "Solution branch & bound, Temps de résolution Branch & Bound,"
    "Nombre de noeuds explorés Branch & Bound, Temps de résolution Backtracking \n";
    my_file << N << "\n";
    while(i<100){
        vector<Arete*> aretes = genere_instances(N, 100, 100, distance_euclidienne);
        g1 = glouton1(N, aretes, 0);
        g2 = glouton2(N, aretes);
        approx1 = deux_approx(N, aretes);
        approx2 = christofides(N, aretes);

        startTime = clock();
        couple = algorithme1(N, aretes);
        t1 = (double (clock()-startTime))/1000;
        bab = get<0>(couple);
        nb_noeuds = get<1>(couple);
        

        startTime = clock();
        backtrck = backtracking(N, aretes);
        t2 = (double (clock()-startTime))/1000;


        cout << t1 <<"s " << t2 << "s" << endl;

        if(valeur_solution(*backtrck) != valeur_solution(*bab)){
            for(Arete *a : aretes){
                a->afficher();
            }
            cout << "Branch & Bound" << valeur_solution(*bab) <<endl;
            affiche_liste(*bab);
            cout << "Approx2" << valeur_solution(*approx2)<<endl;
            affiche_liste(*approx2);
            break;
        }
        else{
            my_file << valeur_solution(*g1) <<","<<valeur_solution(*g2)<<","<< valeur_solution(*approx1)<<",";
            my_file <<valeur_solution(*approx2)<<","<<valeur_solution(*bab)<<","<<t1<<","<<nb_noeuds <<","<< t2<< "\n";
        }
        i++;
    }
    my_file.close();
}