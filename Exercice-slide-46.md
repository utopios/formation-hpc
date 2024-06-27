### QUESTION 1

- Avantages: 
    - Faible latence
    - Haute bande passante
    - Facilité d'implémentation

- Inconvénients:
    - Limité à un seul noeud
    - Difficulté à organiser les données
    - Threads en veille


### QUESTION 2

- Avantages:
    - Scalable au-delà d'un noeud
    - Pas de problème de placement de données
    - Controle explicite des communications
- Inconvenients:
    - Communication explicite
    - Haute latence
    - Difficulté d'implémentation
    - Equilibre de charge difficle

### QUESTION 3

- Avantages:
    - Combinaison des avantages des deux.
    - Chevauchement des calculs et communications

- Inconvenients:
    - Bande passante inter-noeud
    - Surcharge supplémentaire sur l'openMB

### QUESTION 4

- OPENMP
    - Utilisation des directives pour répartir les boucles entre les threads

- MPI
    - Diviser les grilles 3D en sous grilles et attribuler ) un processu MPI


### QUESTION 5

- OPENMP
    - Regrouper les mises à jour des données
    - Utiliser des variables privées et des sections critiques.

- MPI
    - Agréger les données pour réduire les nombre de communications

