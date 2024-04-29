#ifndef ARETE_H
#define ARETE_H

#include <iostream>

using namespace std;

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

int sommet_a_utiliser(vector<Arete*> &ACPM, vector<int> &circuit){
    for(int i=0;i<circuit.size();i++){
        int s = circuit.at(i);
        for(Arete *a : ACPM){
            if(a->sommet1 ==s || a->sommet2 == s) return s;
        }
    }
    return -1;
}

vector<int> circuit_eulerien(vector<Arete*> &ACPM){
    vector<int> circuit, boucle;
    boucle.push_back(ACPM.at(0)->sommet1);
    int sommet_actuel = ACPM.at(0)->sommet1;
    while(true){
        if(sommet_actuel==boucle.at(0) && boucle.size()>1){
            auto it = find(circuit.begin(), circuit.end(), boucle.at(0));
            if(it==circuit.end()) circuit.insert(circuit.begin(), boucle.begin(), boucle.end());
            else circuit.insert(it, boucle.begin(), boucle.end()-1);
            if(ACPM.size()==0) break;
            sommet_actuel = sommet_a_utiliser(ACPM, circuit);
            boucle.clear();
            boucle.push_back(sommet_actuel);
            continue;
        }
        for(int i=0;i<ACPM.size();i++){
            if(ACPM.at(i)->sommet1==sommet_actuel){
                boucle.push_back(ACPM.at(i)->sommet2);
                sommet_actuel=ACPM.at(i)->sommet2;
                ACPM.erase(ACPM.begin()+i);
                break;
            }

            if(ACPM.at(i)->sommet2==sommet_actuel){
                boucle.push_back(ACPM.at(i)->sommet1);
                sommet_actuel=ACPM.at(i)->sommet1;
                ACPM.erase(ACPM.begin()+i);
                break;
            }
        }
    }
    return circuit;
}

void shortcut(int N, vector<int> &circuit){
    unordered_set<int> sommets_explores;
    int i=0;
    while(sommets_explores.size()!=N){
        if(sommets_explores.find(circuit.at(i))==sommets_explores.end()){
            sommets_explores.insert(circuit.at(i));
            i++;
        }
        else{
            circuit.erase(circuit.begin()+i);
        }
    }
    circuit.erase(circuit.begin()+N, circuit.end()-1);
}

#endif