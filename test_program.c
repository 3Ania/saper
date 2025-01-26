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

int test_gra_z_pliku(){
    FILE *plik = fopen("plansza.txt", "r");
    if(plik == NULL){
        printf("Blad przy otwieraniu pliku");
        return -1;
    }
    int wynik = gra_z_pliku(plik);
    if(wynik == 158 || wynik == 160) printf("Testowanie funkcji gra_z_pliku() zakonczona sukcesem\n");
    else if(wynik == 0) printf("Testowanie funkcji gra_z_pliku() zakonczona bledem. Zmienna wynikowa przyjmuje wartosc poczatkowa\n");
    else printf("Testowanie funkcji gra_z_pliku() zakonczona bledem\n");
    return 0;
}

int test_zamien(){
    char znak = 'A';
    if(zamien(znak) != 10){
        printf("Testowanie funkcji zamien() zakonczone bledem przy zamianie litery A na liczbe\n");
        return -1;
    }znak = 'c';
    if(zamien(znak) != 12){
        printf("Testowanie funkcji zamien() zakonczone bledem przy zamianie litery c na liczbe\n");
        return -1;
    }znak = '6';
    if(zamien(znak) != 6){
        printf("Testowanie funkcji zamien() zakonczone bledem przy zamianie liczby 6 z char na int\n");
        return -1;
    }
    printf("Testowanie funkcji zamien() zakonczone sukcesem\n");
    return 0;
}

int test_pobierz_z_pliku(){
    FILE *plik = fopen("plansza.txt", "r");
    if(plik == NULL){
        printf("Blad przy otwieraniu pliku");
        return -1;
    }
    dane d = pobierz_z_pliku(plik);
    if(d.czyPrzejsc != 1){
        printf("Testowanie funkcji pobierz_z_pliku() zakonczone bledem. Nieprawidlowa wartosc zmiennej czyPrzejsc\n");
        return -2;
    }if(d.komenda != 'r'){
        printf("Testowanie funkcji pobierz_z_pliku() zakonczone bledem. Nieprawidlowa wartosc zmiennej komenda\n");
        return -2;
    }if(d.stop != 1){
        printf("Testowanie funkcji pobierz_z_pliku() zakonczone bledem. Nieprawidlowa wartosc zmiennej stop\n");
        return -2;
    }if(d.x != 5){
        printf("Testowanie funkcji pobierz_z_pliku() zakonczone bledem. Nieprawidlowa wartosc zmiennej x\n");
        return -2;
    }if(d.y != 5){
        printf("Testowanie funkcji pobierz_z_pliku() zakonczone bledem. Nieprawidlowa wartosc zmiennej y\n");
        return -2;
    }
    printf("Testowanie funkcji pobierz_z_pliku() zakonczone sukcesem\n");
    return 0;
}

int test_pobierzWyniki(){
    FILE *plik = fopen("w.txt", "r");
    if(plik == NULL){
        printf("Blad przy otwieraniu pliku");
        return -1;
    }
    lb *wynik = malloc(5 * sizeof(lb));
    wynik = pobierzWyniki(plik, wynik);
    if(wynik[0].ilosc != 5) {
        printf("Testowanie funkcji pobierzWyniki() zakonczone bledem. Nieprawidlowe zliczenie ilosci zapisow w pliku\n");
        return -2;
    }else{
        if(strcmp(wynik[0].nazwa, "uz1")!=0){ 
            printf("Testowanie funkcji pobierzWyniki() zakonczone bledem. Nieprawidlowe pobieranie nazwy uzytkownika: %s\n", wynik[0].nazwa);
            return -2;
        }if(strcmp(wynik[3].nazwa, "cd")!=0){ 
            printf("Testowanie funkcji pobierzWyniki() zakonczone bledem. Nieprawidlowe pobieranie nazwy uzytkownika\n");
            return -2;
        }if(wynik[2].wynik != 55){
            printf("Testowanie funkcji pobierzWyniki() zakonczone bledem. Nieprawidlowe pobieranie wyniku uzytkownika\n");
            return -2;
        }if(wynik[4].wynik != 10){
            printf("Testowanie funkcji pobierzWyniki() zakonczone bledem. Nieprawidlowe pobieranie wyniku uzytkownika\n");
            return -2;
        }
    }
    printf("Testowanie funckji pobierzWyniki() zakonczone sukcesem.\n");
    return 0;
}


int main(){
    test_gra_z_pliku();
    test_zamien();
    // test_pobierz_z_pliku();
    test_pobierzWyniki();

    return 0;
}