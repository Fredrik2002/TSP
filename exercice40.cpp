#include <iostream>
#include <vector>
#include <algorithm>
#include "algo1.h"
using namespace std;

vector<Arete> exercice40(){
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

int main(){
    vector<Arete> aretes = exercice40();
    vector<Arete> solution = algorithme1(6, aretes);
    affiche_liste(solution);

}