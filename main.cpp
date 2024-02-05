#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Arete{
    public:
    int sommet1, sommet2, poids;
    Arete(int s1, int s2, int p){
        sommet1 = s1;
        sommet2 = s2;
        poids = p;
    }

    void afficher(){
        cout << sommet1 << "-" << sommet2 << ", Poids: " << poids <<endl;
    }

    bool operator<(const Arete &autre){
        return poids<autre.poids;
    }
};

class Noeud{
    public:
        vector<Arete> aretes; // Listes des arêtes disponibles
        vector<Arete> solution; // ACPM
        int* degres; // Tableau des degrés de chaque sommets
        bool solution_realisable;
        int evaluation; // Poids de l'ACPM
        int N; // Nombre de sommets

    Noeud(vector<Arete> &ar, vector<Arete> &s, int NB_SOMMETS){
        aretes=ar;
        solution=s;
        solution_realisable=false;
        N = NB_SOMMETS;
        degres = new int[N];
        
    }

    bool operator<(const Noeud &autre){//On prend en priorité la solution réalisable
        //return evalue()<autre.evalue();
        if(solution_realisable!=autre.solution_realisable){
            return solution_realisable;
        }
        else{
            return evaluation<autre.evaluation;
        }
    }

    void evalue() {
        for(int i=0;i<N;i++){
            degres[i]=0;
        }
        for(Arete a : solution){
            degres[a.sommet1]+=1;
            degres[a.sommet2]+=1;
        }
        int sommet = max_element(degres, degres+N)-&degres[0];
        int somme = 0;
        for(Arete a : solution){
            somme+=a.poids;
        }
        evaluation = somme;
        if(degres[sommet]==2){// Connexe + Max deg =2 ==> Cycle ==> Solution réalisable
            solution_realisable = true;
        } 
    }
};

void affiche_liste(vector<Arete> &liste){
    for(Arete a : liste){
        a.afficher();
    }
    cout << endl;
}

int trouver_racine(int sommet, int* parent){
    if(parent[sommet]==sommet) return sommet;
    return trouver_racine(parent[sommet], parent);
}

vector<Arete> kruskal(int N, vector<Arete> &aretes, int x0){
    int* parent = new int[N+1];
    vector<Arete> resultat;
    for(int i=0;i<N+1;i++){
        parent[i] = i;
    }
    int i = 0;
    while (resultat.size()!=N-1){
        Arete a = aretes.at(i);
        int r1 = trouver_racine(a.sommet1, parent);
        int r2 = trouver_racine(a.sommet2, parent);

        if (r1!=r2 && a.sommet1!=x0 && a.sommet2!=x0){ // On peut prendre l'arête
            parent[a.sommet2] = a.sommet1; // Lie les composantes connexes
            resultat.push_back(aretes.at(i));
        }
        i++;
    }
    return resultat;

}

vector<Arete> distances_vector(){
    int N = 6;
    int distances[] = {10000,4,7,2,5,4,
                    4,10000,3,2,1,2,
                    7,3,1000,2,6,3,
                    2,2,2,1000,5,3,
                    5,1,6,5,1000,2,
                    4,2,3,3,2,10000};
    vector<Arete> my_vector;
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            Arete a(i,j,distances[i*N+j]);
            my_vector.push_back(a);
        }
    }
    sort(my_vector.begin(), my_vector.end());
    return my_vector;
}

vector<Arete> calcule_solution(int N, int x0, vector<Arete> &aretes){
    // Stocke les arêtes incidentes à x0, les 2 premières sont donc les plus légères
    vector<Arete> aretes_retirees, solution;
    int i=0;
    while(i<aretes.size() && aretes_retirees.size()<2){// On n'a besoin que des 2 arêtes les plus légères
        Arete a = aretes.at(i);
        if(a.sommet1 == x0 || a.sommet2 == x0){
            aretes_retirees.push_back(aretes.at(i));
        } 
        i++;
    }
    if(aretes_retirees.size()>=2){
        solution = kruskal(N-1, aretes, x0);
        solution.push_back(aretes_retirees.at(0));
        solution.push_back(aretes_retirees.at(1));
    }
    
    return solution;
}

vector<Arete> sommet_a_separer(int N, Noeud &n){//Renvoie les arêtes à retirer 
    //(Les arêtes ont toutes un sommet en commun)
    // STRATEGIE DE SEPARATION : A ETUDIER (On prend le sommet de plus haut degré pour l'instant)
    
    int sommet = max_element(n.degres, n.degres+N)-&(n.degres[0]);
    vector<Arete> aretes_a_brancher;
    for(int i=0;i<n.solution.size();i++){
        Arete a = n.solution.at(i);
        if (a.sommet1==sommet || a.sommet2 == sommet) aretes_a_brancher.push_back(n.solution.at(i));
    }
    return aretes_a_brancher;
}

Noeud selection_noeud(vector<Noeud> &liste){ // Strategie de parcours, voir fonction de tri des noeuds
    sort(liste.begin(), liste.end());
    Noeud n = liste.at(0);
    liste.erase(liste.begin());
    return n;
}

vector<Arete> retire_arete(vector<Arete> aretes, Arete a){//Passage par copie OBLIGATOIRE
    for(int i=0;i<aretes.size();i++){
        Arete a1 = aretes.at(i);
        if(a1.sommet1==a.sommet1 && a1.sommet2==a.sommet2){
            aretes.erase(aretes.begin()+i);
            return aretes;
        }
    }
    return aretes;
}

vector<Arete> algorithme1(int N, vector<Arete> &aretes){
    int x0 = 0;
    vector<Arete> best_sol; 
    vector<Arete> solution = calcule_solution(N, x0, aretes);
    vector<Noeud> liste_noeuds;
    Noeud n(aretes, solution, N);
    n.evalue();
    int borne_inf = n.evaluation;
    int borne_sup = 21156135;
    liste_noeuds.push_back(n);
    while(liste_noeuds.size()>0){
        n = selection_noeud(liste_noeuds);
        if(n.solution_realisable){//Solution réalisable
            if(n.evaluation<borne_sup){
                borne_sup=n.evaluation;
                best_sol=n.solution;
            }
            if(n.evaluation==borne_inf){
                return best_sol;
            }
        }
        else if(n.evaluation<borne_sup){
            vector<Arete> branchement = sommet_a_separer(N, n);
            for(Arete a : branchement){
                vector<Arete> new_aretes = retire_arete(n.aretes, a);
                vector<Arete> sol = calcule_solution(N, x0, new_aretes);
                Noeud n_fils(new_aretes, sol, N);
                n_fils.solution = calcule_solution(N, x0, n_fils.aretes);
                n_fils.evalue();
                if(n_fils.solution.size()!=0) liste_noeuds.push_back(n_fils); // Pas assez d'arêtes
            }
        
        }
        
    }
}
int main(){
    vector<Arete> aretes = distances_vector();
    vector<Arete> solution = algorithme1(6, aretes);
    affiche_liste(solution);

}