#include "poczatek_gry.h"
#include "ruch.h"
#include "uproszczenie_kodu.h"
#include "plansza.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

zmienne pobierz_zmienne(zmienne w){
    int stop = 1;
    while(stop){
        printf("Wybierz poziom trudności z dostępnych:\n1. Latwy\n2. Sredni\n3. Trudny\n4. Wlasna plansza\n\n");
        char wybor[20];
        printf("Prosze wpisac nazwe poziomu trudnosci lub liczbe jej odpowiadajaca: ");
        fgets(wybor, 20, stdin);
        wybor[strcspn(wybor, "\n")] = '\0'; // Usuwa znak znak konca linii z pobranego tekstu

        if(stricmp(wybor, "latwy") == 0 || stricmp(wybor, "1") == 0 || stricmp(wybor, "1.") == 0){
            w.n=8, w.m=8, w.p=0.1, w.mnoznik = 1, w.t = 10;
            stop = 0;
        }else if(stricmp(wybor, "sredni") == 0 || stricmp(wybor, "2") == 0 || stricmp(wybor, "2.") == 0){
            w.n=16,w.m=16, w.p=0.15, w.mnoznik = 2, w.t = 40;
            stop = 0;
        }else if(stricmp(wybor, "trudny") == 0 || stricmp(wybor, "3") == 0 || stricmp(wybor, "3.") == 0){
            w.n=16,w.m=32, w.p=0.2, w.mnoznik = 3, w.t = 99;
            stop = 0;
        }else if(stricmp(wybor, "wlasna plansza") == 0 || stricmp(wybor, "4") == 0 || stricmp(wybor, "4.") == 0){
            printf("Proszę podac wielkosc planszy w formacie axb gdzie a to liczba wierszy a b to liczba kolumn: ");
            if(scanf("%dx%d", &w.n, &w.m) == 2) stop = 2; // przechodzi zeby pobrac limit czasu
            else{ // pobieranie od poczatku
                czysc();
                printf("Podano rozmiary planszy w zlym formacie. Prosze zapisac je w formacie axb, gdzie a to liczba wierszy a b to liczba kolumn\n\n");
            }
            while (getchar() != '\n'); // czyszczenie bufora
            if(stop == 2){
                printf("Proszę podac limit czasu na gre: ");
                if(scanf("%d", &w.t) == 1) stop = 0; // pobrano wszystkie dane, mozna wyjsc z petli
                else{ // pobieranie od poczatku
                    czysc();
                    printf("Podano limit czasu w zlym formacie. Prosze podac liczbe calkowita\n\n");
                }
                while (getchar() != '\n'); // czyszczenie bufora
            }
            w.p=0.2;
            if(w.n*w.m < 100) w.mnoznik = 1;
            else if(w.n*w.m < 400) w.mnoznik = 2;
            else w.mnoznik = 3;

        }else{
            czysc();
            printf("Podano zla nazwe poziomu gry. Prosze podac jedna z dostepnych (liczbe lub nazwe trudnosci)\n\n");
        }
    }
    return w;
}

int graUzytkownik(){
    zmienne wartosci = pobierz_zmienne(wartosci);
    int n = wartosci.n, m = wartosci.m, t = wartosci.t;
    double p = wartosci.p;
    int mnoznik = wartosci.mnoznik;
    
    int start = time(NULL);

    czysc();
    printf("------SAPER------\n\n");

    int plansza[n][m];
    int odkryte[n][m]; // odkryte = 1, nie odkryte = 0
    int flagi[n][m]; // flaga = 1, brak flagi = 0
    wypelnij0(n, m, odkryte);
    wypelnij0(n,m,flagi);

    plansza[0][0] = -1;

    int iloscBomb = n*m*p;
    if(rand()%2 == 0) iloscBomb++;

    int ileNieOdkrytych = n*m - iloscBomb, ileNieOdkrytychPol = ileNieOdkrytych;

    wypiszPlansze(n, m, plansza, odkryte, flagi, ileNieOdkrytych);

    int wynik = pobierz_i_wykonaj_komendy(n, m, ileNieOdkrytych, plansza, odkryte, flagi, iloscBomb, mnoznik, p, ileNieOdkrytychPol, start, t, 0);

    return wynik;
}

int gra_z_pliku(FILE *plikPlanszy){
    int n, m, wynik = 0, i, j;
    if(fscanf(plikPlanszy, "%d %d", &n, &m) == 2){}
    else{
        printf("Zle podane dane w pliku.");
        return -1;
    }

    int plansza[n][m];
    int odkryte[n][m]; // odkryte = 1, nie odtyte = 0
    int flagi[n][m]; // flaga = 1, brak flagi = 0
    wypelnij0(n, m, odkryte);
    wypelnij0(n,m,flagi);

    int iloscBomb=0;
    
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(fscanf(plikPlanszy, "%d", &plansza[i][j]) != 1){
                printf("Zle podane dane w pliku");
                return -1;
            }if(plansza[i][j] == 9) iloscBomb++;
        }
    }
    if(rand()%2 == 0) iloscBomb++;
    int ileNieOdkrytych = n*m - iloscBomb, ileNieOdkrytychPol = ileNieOdkrytych;
    int mnoznik;

    if(n*m < 100) mnoznik = 1;
    else if(n*m < 400) mnoznik = 2;
    else mnoznik = 3;

    wynik = pobierz_i_wykonaj_komendy(n, m, ileNieOdkrytych, plansza, odkryte, flagi, iloscBomb, mnoznik, 0, ileNieOdkrytychPol, time(NULL), 0, plikPlanszy);

    fclose(plikPlanszy);
    return wynik;
}
