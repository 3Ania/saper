#include <stdio.h>

#ifndef POCZATEK_GRY_H
#define POCZATEK_GRY_H

typedef struct{
    int n,m, t; // n x m - rozmiary planszy, p - prawdopodobienstwo wystapienia bomby na danym polu, t - maksymalna ilosc czasu w minutach
    double p;
    int mnoznik;
}zmienne;

//pobiera zmienne od uzytkownika (jezeli gra uzytkownik)
zmienne pobierz_zmienne(zmienne w);

//ustawia poczatkowe wartosci dla gry samodzielnej i rozpoczyna gre
int graUzytkownik();

//ustawia poczatkowe wartosci dla gry poprzez plik i rozpoczyna dzialanie kodu
int gra_z_pliku(FILE *plikPlanszy);

#endif