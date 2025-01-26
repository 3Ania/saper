#include "naj_wyniki.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void wypiszLeaderboard(FILE *plik){
    int i = 0, punkty;
    char nazwa[20];
    for(; i < 5 && fscanf(plik, "%s ; %d", &nazwa, &punkty) == 2; i++){
        if(i == 0) printf("Tablica wynikow: \n");
        printf("%d. %s  %d\n", i+1, nazwa, punkty);
    }
}

lb* pobierzWyniki(FILE *plik, lb wyniki[5]){
    int i = 0;
    for(; i < 5 && fscanf(plik, "%s ; %d", &wyniki[i].nazwa, &wyniki[i].wynik) == 2; i++){}
    wyniki[0].ilosc = i;
    return wyniki;
}

void dodajWynik(FILE * plik, int wynik, char nazwa[20]){
    lb *wyniki = malloc(5 * sizeof(lb));
    wyniki = pobierzWyniki(plik, wyniki);
    lb temp;
    int i;
    for(i = 0; i < wyniki[0].ilosc; i++){
        if(wynik > wyniki[i].wynik){
            temp = wyniki[i];
            wyniki[i].wynik = wynik;
            strcpy(wyniki[i].nazwa, nazwa);
            wynik = temp.wynik;
            nazwa = temp.nazwa;
        }
    }
    FILE *p = fopen("wyniki.txt", "w");
    for(i = 0; i < wyniki[0].ilosc; i++) {
        fprintf(p, "%s ; %d\n", wyniki[i].nazwa, wyniki[i].wynik);
    }
    if(i < 5){
        fprintf(p, "%s ; %d\n", nazwa, wynik);
    }
    fclose(p);
}
