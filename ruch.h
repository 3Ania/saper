#ifndef RUCH_H
#define RUCH_H

#include <stdio.h>

typedef struct{
    char komenda;
    int x;
    int y;
    int czyPrzejsc;
    int stop;
}dane;

//zamienia z char na liczbe (gdzie a-10, b-11, c-12 itd);
int zamien(char znak);

//pobiera komende z pliku
dane pobierz_z_pliku(FILE *plikPlanszy);

//pobiera komende od gracza
dane pobierz_od_gracza();

//na podstawie pobranej komendy wykonuje jej dzialanie
int pobierz_i_wykonaj_komendy(int n, int m, int ileNieOdkrytych, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int iloscBomb, int mnoznik, double p, int ileNieOdkrytychPol, int start, int t, FILE *plikPlanszy);

#endif