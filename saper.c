#include "uproszczenie_kodu.h"
#include "plansza.h"
#include "naj_wyniki.h"
#include "podsumowanie.h"
#include "ruch.h"
#include "poczatek_gry.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    srand(time(NULL));
    FILE *plik = fopen("wyniki.txt", "r");
    if(plik == NULL){
        printf("Blad przy otwieraniu pliku");
        return -1;
    }

    int i, j;
    czysc();

    printf("------SAPER------\n\n");
    int wynik = 0;

    FILE *plikPlanszy;

    if(argc == 1) {
        wynik = graUzytkownik();

        printf("Prosze podac nazwe uzytkownika: ");
        char nazwa[20] = "";
        int dlugosc = 21;
        i = 0;
        while(dlugosc > 20){
            if(i == 1) printf("Nazwa uzytkownika moze miec maksymalnie 20 znakow. Prosze podac inna nazwe: ");
            scanf("%s", &nazwa);
            dlugosc = strlen(nazwa);
            i = 1;
        }
        if(nazwa != "") dodajWynik(plik, wynik, nazwa);
        fclose(plik);
        plik = fopen("wyniki.txt", "r");
        if(plik == NULL){
            printf("Blad przy otwieraniu pliku");
            return -1;
        }
        wypiszLeaderboard(plik);
        fclose(plik);
    }else{
        int opt;
        while ((opt = getopt(argc, argv, "f:h")) != -1) {
            switch (opt) {
                case 'f':
                    plikPlanszy = fopen(optarg, "r");
                    if(plikPlanszy == NULL){
                        printf("Blad przy otwieraniu pliku. Sprobuj jeszcze raz.");
                        return -1;
                    }
                    break;
                case 'h':
                    printf("Help: Aby uruchomic gre w trybie czytania z pliku nalezy podac flage -f i podac nazwe pliku.\nAby grac samodzielnie nie nalezy podawac zadnej flagi ani argumentow.\n\n");
                    return 0;
                case '?':
                    printf("Nieznany argument wywolania. Aby otrzymac pomoc wpisz -h.\n\n");
                    return 0;
            }
        }
        wynik = gra_z_pliku(plikPlanszy);
    }
}

// 0 - puste, liczba - liczba do wypisania (ilosc bomb naokolo), 9 - bomba