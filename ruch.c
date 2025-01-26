#include "ruch.h"
#include "plansza.h"
#include "podsumowanie.h"

#include <stdio.h>
#include <time.h>

int zamien(char znak){
    int x;
    if(znak >= 'A' && znak <= 'Z') x = znak - 'A' + 10;
    else if(znak >= 'a' && znak <= 'z') x = znak - 'a' + 10;
    else if(znak >= '0' && znak <= '9') x = znak - '0';
    return x;
}

dane pobierz_z_pliku(FILE *plikPlanszy){
    char komenda = 'x', xc = 'x', yc = 'x';
    int x = 0, y = 0, czy_przejsc = 1;
    int wynik1, stop = 1;

    dane d;

    do {
        komenda = fgetc(plikPlanszy);
    } while (komenda == ' ' || komenda == '\n' || komenda == '\t');
    wynik1 = 1;
    if(komenda == EOF) { // nie ma wiecej do pobrania z pliku
        stop = 0;
        czy_przejsc = 0;
    }else if(wynik1 == 1 && komenda != 'r' && komenda != 'f'){
        printf("Zle podany pierwszy argument. Dostepne opcje:\n r - odkrywanie pola\n f - stawianie flagi\nPodaj poprawna komende.\n");
        czy_przejsc = 0;
    }else{
        wynik1 += fscanf(plikPlanszy, " %c ", &xc);
        if(wynik1 == 2){
            x = zamien(xc);
            wynik1 += fscanf(plikPlanszy, "%c", &yc);
            if(wynik1 == 3){
                y = zamien(yc);
            }else{
                printf("Zle podany trzeci argument. Prosze podac liczbe lub litere z zakresu.\n");
                czy_przejsc = 0;
            }
        }else{
            printf("Zle podany drugi argument. Prosze podac liczbe lub litere z zakresu.\n");
            czy_przejsc = 0;
        }
    }
    d.czyPrzejsc = czy_przejsc;
    d.stop = stop;
    d.x = x;
    d.y = y;
    d.komenda = komenda;
    // printf("%d, %d, %d, %d; ", d.czyPrzejsc, d.stop, d.x, d.y);
    return d;
}

dane pobierz_od_gracza(){
    printf("> ");

    char komenda = 'x', xc = 'x', yc = 'x';
    int x = 0, y = 0, czy_przejsc = 1;
    int wynik1 = scanf("%c ", &komenda);

    dane d;

    if(wynik1 == 1 && komenda != 'r' && komenda != 'f'){
        printf("Zle podany pierwszy argument. Dostepne opcje:\n r - odkrywanie pola\n f - stawianie flagi\nPodaj poprawna komende.\n");
        czy_przejsc = 0;
    }else{
        wynik1 += scanf("%c ", &xc);
        if(wynik1 == 2){
            x = zamien(xc);
            wynik1 += scanf("%c", &yc);
            if(wynik1 == 3){
                y = zamien(yc);
            }else{
                printf("Zle podany trzeci argument. Prosze podac liczbe lub litere z zakresu.\n");
                czy_przejsc = 0;
            }
        }else{
            printf("Zle podany drugi argument. Prosze podac liczbe lub litere z zakresu.\n");
            czy_przejsc = 0;
        }
    }
    while (getchar() != '\n');
    d.czyPrzejsc = czy_przejsc;
    d.komenda = komenda;
    d.stop = 0;
    d.x = x;
    d.y = y;
    return d;
}

int pobierz_i_wykonaj_komendy(int n, int m, int ileNieOdkrytych, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int iloscBomb, int mnoznik, double p, int ileNieOdkrytychPol, int start, int t, FILE *plikPlanszy){
    int stop = 1, czySiePowiodlo;
    char komenda = 'x', xc, yc, enter;
    int x = 0, y = 0, czy_przejsc = 1;
    int wynik1;
    int pierwszePrzejscie = 0;
    int krok;
    dane d;

    while(ileNieOdkrytych > 0 && stop == 1){
        czySiePowiodlo = 0; // nie
        if(p == 0){ //pobierz_z_pliku();
            d = pobierz_z_pliku(plikPlanszy);
            czy_przejsc = d.czyPrzejsc;
            stop = d.stop;
            x = d.x;
            y = d.y;
            komenda = d.komenda;
        }else{
            d = pobierz_od_gracza();
            czy_przejsc = d.czyPrzejsc;
            x = d.x;
            y = d.y;
            komenda = d.komenda;
        }
        
        if(czy_przejsc == 1){
            if(x < 1 || y < 1 || x > n || y > m) printf("Podane koordynaty musza byc dodatnie oraz nie wieksze od rozmiarow planszy. Moga byc podane takze w postaci litery alfabetu, gdzie A = 10, B = 11, itd.\n");
            else{
                x--;
                y--;
                if(komenda == 'f'){
                    if(pierwszePrzejscie == 0) printf("Przed stawianiem flag odkryj pierwsze pole\n");
                    else{
                        if(flagi[x][y] == 0) flagi[x][y] = 1; // stawiamy flage w danym miejscu
                        else flagi[x][y] = 0; // usuwamy flage z pola
                        czySiePowiodlo = 1; //tak
                    }
                }else if(komenda == 'r'){
                    printf("weszlo");
                    czySiePowiodlo = 1; // tak
                    if(pierwszePrzejscie == 0){
                        if(plansza[0][0] == -1) stworzPlansze(n,m,p,plansza, iloscBomb, x, y);
                        pierwszePrzejscie++;
                    }
                    printf("przeszlo ");
                    if(flagi[x][y] == 1){ // jeżeli na danym polu jest flaga, usuwa flage, ale nie odkrywa pola
                        flagi[x][y] = 0;
                    }else{
                        odkryte[x][y] = 1;
                        ileNieOdkrytych--;
                        if(plansza[x][y] == 9){
                            ileNieOdkrytych++; //bomba nie zalicza sie do odkrytych pol
                            stop = 0;
                        }else ileNieOdkrytych = odkryj(x, y, n, m, plansza, odkryte, ileNieOdkrytych);
                    }
                }else{
                    printf("Zle podany pierwszy argument. Dostepne opcje:\n r - odkrywanie pola\n f - stawianie flagi\nPodaj poprawna komende.\n");
                }
            }
        }
        if(p != 0) printf("\n");
        if(czySiePowiodlo == 1){
            krok++;
            if(p != 0){
                wypiszPlansze(n, m, plansza, odkryte, flagi, ileNieOdkrytych);
                printf("Aktualny wynik: %d\n", mnoznik * (ileNieOdkrytychPol - ileNieOdkrytych));
                double aktualny_czas = wypiszCzas(start);
                if (aktualny_czas > t){
                    stop = 0;
                    printf("Koniec czasu! ");
                }
            }
        }
    }
    int wynik = mnoznik * (ileNieOdkrytychPol - ileNieOdkrytych);
    wypiszWynik(p, ileNieOdkrytych, wynik, krok);

    return wynik;
}