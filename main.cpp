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

    vector<Arete*>* g1 = new vector<Arete*>();
    vector<Arete*>* g2 = new vector<Arete*>();
    vector<Arete*>* approx1 = new vector<Arete*>();
    vector<Arete*>* approx2 = new vector<Arete*>();

    tuple<double, int> couple;
    tuple<double, int> couple2;
    int i=0;
    int N=8;
    int m = N*(N-1)/2;
    int nb_noeuds, nb_noeuds2, backtrck, s1, s2, held_karp;
    ofstream my_file_approx, my_file_exacte, instances;
    clock_t startTime;
    double t1, t2, t3, t4;
    my_file_approx.open("datas_approx.csv");
    my_file_exacte.open("datas_exacte.csv");
    instances.open("instances.txt");
    my_file_approx << "Solution exacte, Solution Gloutonne 1, Solution Gloutonne 2, Solution 2-Approximation, Solution 3/2-Approximation \n";
    my_file_exacte << "Temps de résolution Backtracking, Temps de résolution Branch & Bound1 (Orienté arête),"
    "Nombre de noeuds explorés Branch & Bound1, Temps de résolution Branch & Bound2 (Orienté sommet),"
    "Nombre de noeuds explorés Branch & Bound2, Temps de résolution programmation dynamique, Temps de résolution PLNE \n";
    my_file_approx << N << "\n";
    my_file_exacte << N << "\n";
    Arete* aretes2 = new Arete[m];
    while(i<100){
        vector<Arete*> aretes = genere_instances(N, 100, 100, distance_de_manhattan);
        double* matrice = matrice_distance(N, aretes);
        for(int i=0;i<m;i++){
            aretes2[i] = *(aretes.at(i));
        }
        // SOLUTIONS APPROCHEES
        g1 = glouton1(N, aretes, 0);
        g2 = glouton2(N, aretes);
        approx1 = deux_approx(N, aretes);
        approx2 = christofides(N, aretes);

        //SOLUTIONS EXACTES
        
        startTime = clock();
        backtrck = valeur_solution(*backtracking(N, aretes));
        t1 = (double (clock()-startTime))/1000;
        //cout << t1 << "s ";
        
        startTime = clock();
        couple = lance_profondeur(N, aretes2);
        t2 = (double (clock()-startTime))/1000;
        s1 = get<0>(couple);
        nb_noeuds = get<1>(couple);
        //cout << t2 << "s, "<<nb_noeuds<<" noeuds ";
        t2 = 0;
        
    
        startTime = clock();
        couple2 = lance_profondeur2(N, matrice);
        t3 = (double (clock()-startTime))/1000;
        nb_noeuds2 = get<1>(couple2);
        //cout <<t3 <<"s, "<<nb_noeuds2<<" noeuds "<< endl;
        s2 = get<0>(couple2);
        

        startTime = clock();
        held_karp = 0;
        t4 = (double (clock()-startTime))/1000;

        if(s1!=s2 || s2!=backtrck){
            
            for(Arete *a : aretes){
                a->afficher();
            }
            cout << "Branch & Bound1 :" << s1 <<endl;
            cout << "backtrck :" << backtrck<<endl;
            cout << "Branch & Bound 2 :" << s2<<endl;
        }
        else{
            my_file_approx<<backtrck<<"," << valeur_solution(*g1) <<","<<valeur_solution(*g2)<<","<< valeur_solution(*approx1)<<","<<valeur_solution(*approx2)<<"\n";
            my_file_exacte <<t1 <<","<< t2<< ","<<nb_noeuds<<",";
            my_file_exacte << t3 <<","<<nb_noeuds2<<","<<t4<<",\n";
        }
        i++;
    }
    my_file_approx.close();
    my_file_exacte.close();
    cout << "Exited successfully" << endl;
}