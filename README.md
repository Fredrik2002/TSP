### Spoiler : Ceci n'est pas un README

Rappel : 
- Glouton 1 part du sommet 0 et va toujours au plus proche voisin par encore exploré. 
    Complexité asymptotique : O(n²)
- Glouton 2 appelle Glouton 1 sur tous les sommets de départ possible et garde la meilleure solution :
    Complexité asymptotique : O(n<sup>3</sup>)

__Analyse de Données__
- Branch & Bound : Corrélation entre le nombre de noeuds explorés et le temps de résolution
- Branch & Bound vs Backtracking : Corrélation entre les 2 temps de résolutions
- Solutions approchées vs Solution exacte : Écart entre les solutions approchées et la solution exactes
