#include "podsumowanie.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

double wypiszCzas(int start){
    int koniec = time(NULL);
    int min = (koniec-start) / 60;
    int sec = (koniec-start)%60;
    char *minuta;
    char *sekunda;
    if(min == 1) minuta = "minuta";
    else if(min % 10 < 5 && min % 10 > 1 && min / 10 % 10 != 1) minuta = "minuty";
    else minuta = "minut";
    if(sec == 1) sekunda = "sekunda";
    else if(sec % 10 < 5 && sec % 10 > 1 && sec / 10 % 10 != 1) sekunda = "sekundy";
    else sekunda = "sekund";
    if(min > 0) printf("Czas trwania gry: %d %s i %d %s\n", min, minuta, sec, sekunda);
    else printf("Czas trwania gry: %d %s\n", sec, sekunda);
    return (double)(koniec-start) / 60;
}

void wypiszWynik(double p, int ileNieOdkrytych, int wynik, int krok){
    if(p != 0) { 
        if(ileNieOdkrytych == 0){
            printf("Wygrales!!! Ilosc zdobytych punktow: %d\n", wynik);
        }else{
            printf("Przegrales!\n");
        }
        printf("Wynik: %d\n", wynik);
    }else{
        printf("%d %d ", krok, wynik);
        if(ileNieOdkrytych == 0){
            printf("1\n");
        }else{
            printf("0\n");
        }
    }
}