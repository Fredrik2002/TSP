#include <bits/stdc++.h>

using namespace std;
#define n 5


int tsp_dp(double graphe[][n], int s) {
    vector<int> sommets;
    for (int i = 0; i < n; i++) {
        if (i != s)
            sommets.push_back(i);
    }
    double cout_min = 1000000;
    while(next_permutation(sommets.begin(), sommets.end())) {
        double cout_courrant = 0;
        int j = s;
        for (int i = 0; i < sommets.size(); i++) {
            cout_courrant += graphe[j][sommets[i]];
            j = sommets[i];
        }
        cout_courrant += graphe[j][s];
        cout_min = min(cout_min, cout_courrant);
        return cout_min;
	}
}

int main() {
    double graphe[][n] = {{0, 71,42,79,66}, {71, 0, 75,70,5}, {42,75,0,37,70}, {79,70,37,0,65}, {66,5,70,65,0}};                      
    int s = 4;
    cout << tsp_dp(graphe, s) << endl;
    return 0;
}