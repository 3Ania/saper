#ifndef PLANSZA_H
#define PLANSZA_H

//tworzy losowa plansze (przy wyborze gry samodzielnej)
void stworzPlansze(int n, int m, double p, int plansza[n][m], int iloscBomb, int a, int b);

//odkrywa pole po wpisaniu komendy r x x
int odkryj(int x, int y, int n, int m, int plansza[n][m], int odkryte[n][m], int ile);

//wypisuje plansze na konsole
void wypiszPlansze(int n, int m, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int ileNieOdkrytych);

#endif