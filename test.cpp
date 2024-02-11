#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include "algo1.h"
#include "glouton.h"
#include "backtracking.h"
#include "2-approx.h"

using namespace std;

int main(){
    ifstream fin;

    string line, str;
    fin.open("test.txt");
    int i=0;
    int* H = new int[3];
    vector<Arete*> aretes;

    while (getline(fin, line)) {
        stringstream ss(line);
        while(getline(ss, str, ',')){
            H[i%3]=stoi(str);
            i++;
        }
        aretes.push_back(new Arete(H[0], H[1], H[2]));
        
    }

    for(Arete *a : aretes){
        a->afficher();
    }
    int N = 10;
    vector<Arete*>* solution = new vector<Arete*>();
    solution = glouton1(N, aretes, 0);
    affiche_liste(*solution);
    cout <<endl << "Glouton 1 :" << valeur_solution(*solution);

    solution = glouton2(N, aretes);
    affiche_liste(*solution);
    cout <<endl << "Glouton 2 :" << valeur_solution(*solution);

    solution = deux_approx(N, aretes);
    affiche_liste(*solution);
    cout <<endl << "2-approx :" << valeur_solution(*solution);

    solution = backtracking(N, aretes);
    affiche_liste(*solution);
    cout <<endl << "Backtracking :" << valeur_solution(*solution);

    solution = algorithme1(N, aretes);
    cout <<endl << "Algo 1 :" << valeur_solution(*solution);
 
    // Close the file
    fin.close();


}