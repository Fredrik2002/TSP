#include <iostream>
#include "branch_and_bound2.h"

using namespace std;

class Noeud3{
    static double* distances;
    public:
        int p;
        int* solution;
        int* sommets_places; // liste des sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        int N, m; // Nombre de sommets, Nombre d'aretes
};


int main(){
    int N = 15;
    double* d = new double[N];
    
    Noeud2::N = N;
    Noeud2::distances = d;
    Noeud2::m = N*(N-1)/2;

    cout << sizeof(Noeud2) << endl;
    cout << sizeof(int) << endl;
    cout << sizeof(Noeud3) << endl;
    cout << sizeof(double*) << endl;

    Noeud2::N = 12;
    Noeud2::distances = d;
    Noeud2::m = N*(N-1)/2;

    cout << sizeof(Noeud2) << endl;
    cout << sizeof(int) << endl;
    cout << sizeof(Noeud3) << endl;
    cout << sizeof(double*) << endl;
}
    
