#ifndef BRANCH_AND_BOUND
#define BRANCH_AND_BOUND

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
    Arete(){}
    Arete(int s1, int s2, double p, int i){
        sommet1 = s1;
        sommet2 = s2;
        poids = p;
        id = i;
    }

    void afficher(){
        cout << sommet1 << "," << sommet2 << ", " << poids << endl;
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

bool comparateur_pointeur(const Arete* a, const Arete* b){
    return a->poids < b-> poids;
}

class Noeud{
    public:
        static unordered_set<string> set; 
        static int x0;
        static int N, m;
        static int nb_noeuds_crees, nb_noeuds_detruits;
        static Arete* aretes; // Listes des arêtes disponibles 

        int* solution; // tableau des arêtes de la solution
        int* degres; // Tableau des degrés de chaque sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        int sommet_degre_max; // Sommet de degré maximum
        string hashcode;
        

    Noeud(Arete* &ar,  int NB_SOMMETS, int x){
        //Static
        unordered_set<string> s;
        set = s;
        aretes=ar;
        nb_noeuds_crees = 1;
        nb_noeuds_detruits = 0;
        N = NB_SOMMETS;
        m=N*(N-1)/2;
        x0 = x;

        solution_realisable=false;
        degres = new int[N]();
        solution = new int[N]();
        evaluation = 0;
        solution[N-3] = -1;
        hashcode = string(m, '0');
        evalue();
    }

    Noeud(Noeud &n, int a){
        solution_realisable=false;
        degres = new int[N]();
        solution = new int[N];
        solution[N-3] = -1;
        hashcode = n.hashcode;
        evaluation = 0;
        hashcode[a]='1';
        if(set.find(hashcode)==set.end()){
            evalue();
            set.insert(hashcode); 
            nb_noeuds_crees++;
        } 
        else {
            evaluation = 1000000000;
            sommet_degre_max = -1;
        }
    }

    ~Noeud(){
        nb_noeuds_detruits++;
        delete[] degres;
        delete[] solution;
    }

    void afficher(){
        cout << "Evaluation du noeud :" << evaluation <<endl;
        cout << "Solution réalisable :"<<solution_realisable<<endl;
        for(int i=0;i<N;i++){
            aretes[solution[i]].afficher();
        }
        cout << "Hashcode :" << hashcode << endl << endl;
    }

    bool operator<(const Noeud &autre){
        return evaluation<autre.evaluation;
    }

    void evalue() {
        calcule_solution();
        if(solution[N-3]==-1) return;
        sommet_degre_max = max_element(degres, degres+N)-&degres[0];
        int sommet_min = min_element(degres, degres+N)-&degres[0];
        
        if(degres[sommet_degre_max]==2 && degres[sommet_min]==2){ //Connexe + tous les sommets de degré 2 => solution réalisable
            solution_realisable = true;
        }
    }

    void kruskal(){
        int parent[N];
        int p=0;
        for(int i=0;i<N;i++){
            parent[i] = i;
        }
        for(int i=0;i<m;i++){
            Arete a = aretes[i];
            int r1 = trouver_racine(a.sommet1, parent);
            int r2 = trouver_racine(a.sommet2, parent);

            if (r1!=r2 && a.sommet1!=x0 && a.sommet2!=x0 && hashcode[i]=='0'){ // On peut prendre l'arête
                parent[r2] = r1; // Lie les composantes connexes
                solution[p] = i;
                degres[a.sommet1]++;
                degres[a.sommet2]++;
                evaluation += a.poids;
                p++;
            }
            if(p==N-2) break;
        }
    }

    void calcule_solution(){
        // Stocke les arêtes incidentes à x0, les 2 premières sont donc les plus légères
        vector<int> aretes_retirees;
        int i=0;
        while(i<m && aretes_retirees.size()<2){// On n'a besoin que des 2 arêtes les plus légères
            Arete a = aretes[i];
            if((a.sommet1 == x0 || a.sommet2 == x0) && hashcode[i]=='0'){
                aretes_retirees.push_back(i);
                degres[a.sommet1]++;
                degres[a.sommet2]++;
                evaluation += a.poids;
            }
            i++;
        }
        if(aretes_retirees.size()>=2){
            kruskal();
            if(solution[N-3]!=-1){//On a un arbre couvrant
                solution[N-2]=aretes_retirees.at(0);
                solution[N-1]=aretes_retirees.at(1);
            }
        }
    }
};


vector<int> sommet_a_separer(int N, Noeud &n){//Renvoie les arêtes à retirer 
    //(Les arêtes ont toutes un sommet en commun)
    // STRATEGIE DE SEPARATION : A ETUDIER (On prend le sommet de plus haut degré pour l'instant)
    int sommet = n.sommet_degre_max;
    assert(sommet!=-1);
    vector<int> aretes_a_brancher;
    for(int i=0;i<N;i++){
        Arete a = n.aretes[n.solution[i]];
        if (a.sommet1==sommet || a.sommet2 == sommet) aretes_a_brancher.push_back(n.solution[i]);
    }
    return aretes_a_brancher;
}

void insertion_dichotomique(vector<Noeud> &liste, Noeud &n){
    auto it = lower_bound(liste.begin(), liste.end(), n);
    liste.insert(it, n);
}

void branch_and_bound_profondeur(Noeud* &n, int N, Arete* &aretes, double &borne_sup,
std::chrono::time_point<std::chrono::high_resolution_clock> temps_depart, double timeout){
    vector<int> branchement = sommet_a_separer(N, *n);
    for(int a : branchement){
        Noeud* n_fils = new Noeud(*n, a);
        if(n_fils->solution[N-3]!=-1 && n_fils->evaluation<borne_sup){
            
            if(n_fils->solution_realisable){
                borne_sup=n_fils->evaluation;
            }
            else{
                branch_and_bound_profondeur(n_fils, N, aretes, borne_sup, temps_depart, timeout);
            }
        }
        delete n_fils;
        if(std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-temps_depart).count()>timeout){
            borne_sup = -1;
            return;
        }
    }
}

tuple<double, int> lance_profondeur(int N, Arete* &aretes, 
std::chrono::time_point<std::chrono::high_resolution_clock> temps_depart, double timeout=100000, double borne_sup=13245678){
    tuple<double, int> to_return;
    int nb_noeuds_explores = 0;
    borne_sup+=0.0001;
    int best_x0 = 0;
    double best_sol = 0;
    for(int i=0;i<N;i++){
        Noeud* n = new Noeud(aretes, N, i);
        if(best_sol<n->evaluation){
            best_x0 = i;
            best_sol = n->evaluation;
        }
        delete n;
    }
    Noeud* n = new Noeud(aretes, N, best_x0);
    if(n->solution_realisable) borne_sup=n->evaluation;
    branch_and_bound_profondeur(n, N, aretes, borne_sup, temps_depart, timeout);
    borne_sup==-1 ? nb_noeuds_explores=-1 :nb_noeuds_explores = n->nb_noeuds_crees; 
    delete n;
    return make_tuple(borne_sup, nb_noeuds_explores);
}

unordered_set<string> set2;
Arete* a;

Arete* Noeud::aretes = a;
unordered_set<string> Noeud::set = set2;
int Noeud::N = 0;
int Noeud::m = 0;
int Noeud::x0 = 0;
int Noeud::nb_noeuds_crees = 0;
int Noeud::nb_noeuds_detruits = 0;



#endif