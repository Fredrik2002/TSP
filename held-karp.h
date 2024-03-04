#ifndef HELD_KARP_H
#define HELD_KARP_H

#include <bits/stdc++.h>

using namespace std;


double tsp_dp(int N, double* &graphe, int s) {
    vector<int> sommets;
    for (int i = 0; i < N; i++) {
        if (i != s)
            sommets.push_back(i);
    }
    double cout_min = 1000000;
    while(next_permutation(sommets.begin(), sommets.end())) {
        double cout_courrant = 0;
        int j = s;
        for (int i = 0; i < sommets.size(); i++) {
            cout_courrant += graphe[j*N+sommets[i]];
            j = sommets[i];
        }
        cout_courrant += graphe[j*N+s];
        cout_min = min(cout_min, cout_courrant);
        return cout_min;
	}
}

#endif