#include "plansza.h"
#include "uproszczenie_kodu.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void stworzPlansze(int n, int m, double p, int plansza[n][m], int iloscBomb, int a, int b){
    int i, j;
    wypelnij0(n, m, plansza);

    printf("tworze... %d, ", iloscBomb);
    i = 0;
    int x,y;
    while(i < iloscBomb){
        x = rand()%n;
        y = rand()%m;
        if((plansza[x][y] == 0 || plansza[x][y] == -1) && (abs(x-a) > 1 || abs(y-b) > 1)){ // puste pole
            printf("%d-%d, ", x, y);
            plansza[x][y] = 9; // bomba
            i++;
        }
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(plansza[i][j] == 9){ // jeżeli jesteśmy na polu bomby
                for(x = i-1; x < i+2; x++){
                    for(y = j-1; y < j+2; y++){
                        if(x >= 0 && y >= 0 && x < n && y < m && plansza[x][y] != 9){ // jeżeli pole w granicach planszy i nie jest bomba
                            plansza[x][y]++; // zwiekszamy nr pola (ilosc otaczających je bomb)
                        }
                    }
                }
            }
        }
    }
}

int odkryj(int x, int y, int n, int m, int plansza[n][m], int odkryte[n][m], int ile){
    if(odkryte[x][y] == 0){
        odkryte[x][y] = 1; // odkrywamy aktualne pole
        ile--; //o jedno mniej zakrytych pol
    }

    if(plansza[x][y] == 0){
        int i, j;
        for(i = x-1; i < x+2; i++){
            for(j = y-1; j < y+2; j++){
                if(i >= 0 && j >= 0 && i < n && j < m){ // jeżeli pole w granicach planszy
                    if(plansza[i][j] == 0 && odkryte[i][j] == 0){
                        ile = odkryj(i, j, n, m, plansza, odkryte, ile);
                    }else if(plansza[i][j] != 9 && odkryte[i][j] == 0){ // jeżeli pole to nie bomba, ale jeszcze nie odkryte, to odkrywam
                        odkryte[i][j] = 1;
                        ile--;
                    }
                }
            }
        }
    }

    return ile;
}

void wypiszPlansze(int n, int m, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int ileNieOdkrytych){
    czysc();
    printf("------SAPER------\n\n");

    int i, j;
    if(m < 36){
        printf("   ");
        for(j = 1; j < m+1; j++) j < 10 ? printf("\033[0;90m%d \033[0m", j) : printf("\033[0;90m%c \033[0m", 'A' + j - 10);
    }
    printf("\n");
    for(i = 0; i < n; i++){
        if(n < 36) i < 9 ? printf("\033[0;90m%d  \033[0m", i+1) : printf("\033[0;90m%c  \033[0m", 'A' + i - 9);
        for(j = 0; j < m; j++){
            if(odkryte[i][j] == 1){
                if(plansza[i][j] == 9){
                    printf("\033[0;31m* \033[0m");
                }else if(plansza[i][j] == 0){
                    printf("\033[0;90m_ \033[0m");
                }else{
                    printf("\033[0;36m%d \033[0m", plansza[i][j]);
                }
            }else if(flagi[i][j] == 1){
                printf("\033[0;35mF \033[0m");
            }else printf("# ");
        }printf("\n");
    }
    printf("\nPozostalo %d pol do odkrycia\n", ileNieOdkrytych);
}