#ifndef ALGO1_H
#define ALGO1_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <tuple>

using namespace std;

int trouver_racine(int sommet, int* parent){
    if(parent[sommet]==sommet) return sommet;
    return trouver_racine(parent[sommet], parent);
}

int fonction_de_hachage(int a, int b, int N){
    return -a*(a+3)/2+a*N+b-1;
}



class Arete{
    public:
    int sommet1, sommet2, id;
    double poids;
    Arete(int s1, int s2, double p, int N){
        sommet1 = s1;
        sommet2 = s2;
        poids = p;
        id = fonction_de_hachage(sommet1, sommet2, N);
    }

    void afficher(){
        cout << sommet1 << "-" << sommet2 << ", Poids: " << poids <<" - ID :" << id << endl;
    }

    bool operator<(const Arete &autre){
        return poids<autre.poids;
    }

    int hash(int N){
        return fonction_de_hachage(sommet1, sommet2, N);
    }
};

void affiche_liste(vector<Arete*> &liste){
    for(Arete* a : liste){
        a->afficher();
    }
}


vector<Arete*>* copie_solution_dans_le_tas(vector<Arete*> &liste){
    vector<Arete*>* s = new vector<Arete*>();
    for(Arete *a : liste){
        s->push_back(a);
    }
    return s;
}

bool comparateur_pointeur(const Arete* a, const Arete* b){
    return a->poids < b-> poids;
}

vector<Arete*> kruskal(int N, vector<Arete*> &aretes, int x0){//Prend en paramètre la liste TRIEE des arêtes
    //N = le sommet de sommets (en comptant x0)
    int* parent = new int[N];
    vector<Arete*> resultat, solution_vide;
    for(int i=0;i<N;i++){
        parent[i] = i;
    }
    for(int i=0;i<aretes.size();i++){
        Arete* a = aretes.at(i);
        int r1 = trouver_racine(a->sommet1, parent);
        int r2 = trouver_racine(a->sommet2, parent);

        if (r1!=r2 && a->sommet1!=x0 && a->sommet2!=x0){ // On peut prendre l'arête
            parent[r2] = r1; // Lie les composantes connexes
            resultat.push_back(aretes.at(i));
        }
        if(resultat.size()==N-2) break;
    }
    delete parent;
    if(resultat.size()==N-2){
        return resultat;
    }
    else return solution_vide;
}

vector<Arete*> calcule_solution(int N, int x0, vector<Arete*> &aretes){//Kruskal a interdiction de prendre les arêtes de x0, pas la peine de retirer les arêtes donc -> Passage par référence
    // Stocke les arêtes incidentes à x0, les 2 premières sont donc les plus légères
    vector<Arete*> aretes_retirees, solution;
    int i=0;
    while(i<aretes.size() && aretes_retirees.size()<2){// On n'a besoin que des 2 arêtes les plus légères
        Arete* a = aretes.at(i);
        if(a->sommet1 == x0 || a->sommet2 == x0){
            aretes_retirees.push_back(aretes.at(i));
        }
        i++;
    }
    if(aretes_retirees.size()>=2){
        solution = kruskal(N, aretes, x0);
        if(solution.size()!=0){
            solution.push_back(aretes_retirees.at(0));
            solution.push_back(aretes_retirees.at(1));
        }
    }
    return solution;
}

class Noeud{
    public:
        vector<Arete*> aretes; // Listes des arêtes disponibles
        vector<Arete*> solution; // ACPM
        vector<Arete*> aretes_interdites;
        int* degres; // Tableau des degrés de chaque sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        int N; // Nombre de sommets
        string hashcode;
        int id;
        int x0;

    Noeud(vector<Arete*> &ar,  int NB_SOMMETS, vector<Arete*> aretes_inter, int &sommet_interdit){//Passer les arêtes interdites par copie également
        aretes=ar;
        solution_realisable=false;
        N = NB_SOMMETS;
        degres = new int[N]();
        aretes_interdites = aretes_inter;
        id =0;
        x0 = sommet_interdit;
        
    }

    string hash(){
        string hashcode(N*(N-1)/2, '0');
        for(Arete* ar : aretes_interdites){
            int e = fonction_de_hachage(ar->sommet1, ar->sommet2, N);
            hashcode[e] = '1';
        }
        return hashcode;
    }

    void afficher(){
        cout << "Evaluation du noeud :" << evaluation <<endl;
        cout << "Solution réalisable :"<<solution_realisable<<endl;
        affiche_liste(solution);
        cout << "Aretes interdites:" <<endl;
        affiche_liste(aretes_interdites);
        cout << endl << endl;
    }

    bool operator<(const Noeud &autre){
        return evaluation<autre.evaluation;
        
    }

    void evalue() {
        solution = calcule_solution(N, x0, aretes);
        hashcode = hash();
        for(Arete* a : solution){
            degres[a->sommet1]+=1;
            degres[a->sommet2]+=1;
        }
        int sommet_max = max_element(degres, degres+N)-&degres[0];
        int sommet_min = min_element(degres, degres+N)-&degres[0];
        double somme = 0;
        for(Arete* a : solution){
            somme+=(*a).poids;
        }
        evaluation = somme;
        if(degres[sommet_max]==2 && degres[sommet_min]==2){ // On est sur d'avoir une union de cycle
            solution_realisable=true;
            vector<Arete*> copie_solution = solution;
            int sommet_de_depart = copie_solution.at(0)->sommet1;
            int sommet_actuel = copie_solution.at(0)->sommet2;
            copie_solution.erase(copie_solution.begin());
            while(sommet_de_depart!=sommet_actuel){
                for(int i=0;i<copie_solution.size();i++){
                    Arete* a = copie_solution.at(i);
                    if(a->sommet1==sommet_actuel){
                        sommet_actuel = a->sommet2;
                        copie_solution.erase(copie_solution.begin()+i);
                        break;
                    }
                    if(a->sommet2==sommet_actuel){
                        sommet_actuel = a->sommet1;
                        copie_solution.erase(copie_solution.begin()+i);
                        break;
                    }
                }
            }
            solution_realisable = copie_solution.size()==0; // Si on a épuisé toutes les arêtes, on a bien qu'un seul cycle
        }
    }
};


vector<Arete*> sommet_a_separer(int N, Noeud &n){//Renvoie les arêtes à retirer 
    //(Les arêtes ont toutes un sommet en commun)
    // STRATEGIE DE SEPARATION : A ETUDIER (On prend le sommet de plus haut degré pour l'instant)
    /*int sommet = N+1;
    for(int i=0;i<N;i++){
        if(n.degres[i]>2 && (sommet==N+1 || n.degres[i]<n.degres[sommet])){
            sommet = i;
            if (n.degres[i]==3) break;
        }
    }*/
    
    int sommet = max_element(n.degres, n.degres+N)-&(n.degres[0]);
    vector<Arete*> aretes_a_brancher;
    for(int i=0;i<n.solution.size();i++){
        Arete* a = n.solution.at(i);
        if (a->sommet1==sommet || a->sommet2 == sommet) aretes_a_brancher.push_back(n.solution.at(i));
    }
    return aretes_a_brancher;
}

Noeud selection_noeud(vector<Noeud> &liste){ // Strategie de parcours, voir fonction de tri des noeuds
    //sort(liste.begin(), liste.end());
    Noeud n = liste.at(0);
    liste.erase(liste.begin());
    return n;
}

vector<Arete*> retire_arete(vector<Arete*> aretes, Arete &a){//Passage par copie OBLIGATOIRE
    for(int i=0;i<aretes.size();i++){
        Arete* a1 = aretes.at(i);
        if(a1->sommet1==a.sommet1 && a1->sommet2==a.sommet2){
            aretes.erase(aretes.begin()+i);
            return aretes;
        }
    }
    return aretes;
}

void insertion_dichotomique(vector<Noeud> &liste, Noeud &n){
    auto it = lower_bound(liste.begin(), liste.end(), n);
    liste.insert(it, n);
}

tuple<vector<Arete*>*, int> algorithme1(int N, vector<Arete*> &aretes){
    tuple<vector<Arete*>*, int> to_return;
    int x0 = 0;
    int nb_noeuds_explores = 0;
    double borne_sup = 12345678;
    unordered_set<string> set;
    vector<Arete*> best_sol, aretes_interdites; 
    vector<Noeud> liste_noeuds;
    Noeud n(aretes, N, aretes_interdites, x0);
    n.evalue();
    liste_noeuds.push_back(n);

    while(liste_noeuds.size()>0){
        n = selection_noeud(liste_noeuds);
        nb_noeuds_explores++;
        if(n.solution_realisable){//Solution réalisable
            cout << "Nb noeuds " << nb_noeuds_explores <<endl;
            to_return = make_tuple(copie_solution_dans_le_tas(n.solution), nb_noeuds_explores);
            return to_return;
            
        }
        
        vector<Arete*> branchement = sommet_a_separer(N, n);
        for(Arete* a : branchement){
            vector<Arete*> new_aretes = retire_arete(n.aretes, *a);
            Noeud n_fils(new_aretes, N, n.aretes_interdites, x0);
            n_fils.aretes_interdites.push_back(a);
            n_fils.evalue();
            if(n_fils.solution.size()!=0 && set.find(n_fils.hashcode)==set.end() && n_fils.evaluation<borne_sup){
                if(n_fils.solution_realisable){
                    borne_sup=n_fils.evaluation;
                    auto it = lower_bound(liste_noeuds.begin(), liste_noeuds.end(), n_fils);
                    liste_noeuds.erase(it, liste_noeuds.end());
                    liste_noeuds.push_back(n_fils);
                }
                else{
                    insertion_dichotomique(liste_noeuds,n_fils);
                }
                set.insert(n_fils.hashcode); 
            }
        }
        
    }
}

#endif