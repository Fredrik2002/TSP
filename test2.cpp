#include <iostream>

using namespace std;

class Noeud{
    public :
        int N;
        int* solution;

        Noeud(int Na){
            N = Na;
            solution = new int[N];
            solution [0] = 0;
        }
        
        Noeud(Noeud &n){
            N = n.N;
            solution = new int[N];
            copy(n.solution, n.solution+N, solution);
            int p=1;
            solution[p]=1;
            
        }
};

int main(){
    int N = 5;
    int m = N*(N-1)/2;
    double distances[25] = {0,104,30,48,20,
                        104,0,84,152,96,
                        30,84,0,68,12,
                        48,152,68,0,56,
                        20,96,12,56,0};
    //branch_and_bound2(N, distances);
    Noeud2 n(distances, N);
    Noeud2 n2(n, 1);
    Noeud2 n3(n, 3);
    Noeud2 n4(n3, 2);
    cout << "finished" << endl;
}
    
