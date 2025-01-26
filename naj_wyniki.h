#include <stdio.h>

#ifndef NAJ_WYNIKI_H
#define NAJ_WYNIKI_H

typedef struct{
    char nazwa[20];
    int wynik;
    int ilosc;
}lb;

//wypisuje liste 5 najlepszych wynikow
void wypiszLeaderboard(FILE *plik);

//pobiera wyniki z pliku
lb* pobierzWyniki(FILE *plik, lb wyniki[5]);

//dodaje aktualny wynik gracza do listy (jezeli jest lepszy od ktoregos istniejacego)
void dodajWynik(FILE * plik, int wynik, char nazwa[20]);

#endif