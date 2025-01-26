#include "uproszczenie_kodu.h"

#include <stdio.h>

void czysc(){ //czysci konsole
    printf("\033[H\033[J");
}

void wypelnij0(int n, int m, int tab[n][m]){ // wypelnia cala tablice samymi zerami
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            tab[i][j] = 0;
        }
    }
}