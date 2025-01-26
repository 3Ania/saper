#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>

typedef struct{
    int n,m, t; // n x m - rozmiary planszy, p - prawdopodobienstwo wystapienia bomby na danym polu, t - maksymalna ilosc czasu w minutach
    double p;
    int mnoznik;
}zmienne;

typedef struct{
    char nazwa[20];
    int wynik;
    int ilosc;
}leaderboard;

typedef struct{
    char komenda;
    int x;
    int y;
    int czyPrzejsc;
    int stop;
}dane;

void czysc(){ //czysci konsole
    printf("\033[H\033[J");
}

void wypelnij0(int n, int m, int tab[n][m]){ // wypelnia cala tablice samymi zerami
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            tab[i][j] = 0;
        }
    }
}

void stworzPlansze(int n, int m, double p, int plansza[n][m], int iloscBomb, int a, int b){
    int i, j;
    wypelnij0(n, m, plansza);

    i = 0;
    int x,y;
    while(i < iloscBomb){
        x = rand()%n;
        y = rand()%m;
        if(plansza[x][y] == 0 && (abs(x-a) > 1 && abs(y-b) > 1)){ // puste pole
            plansza[x][y] = 9; // bomba
            i++;
        }
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(plansza[i][j] == 9){ // jeżeli jesteśmy na polu bomby
                for(x = i-1; x < i+2; x++){
                    for(y = j-1; y < j+2; y++){
                        if(x >= 0 && y >= 0 && x < n && y < m && plansza[x][y] != 9){ // jeżeli pole w granicach planszy i nie jest bomba
                            plansza[x][y]++; // zwiekszamy nr pola (ilosc otaczających je bomb)
                        }
                    }
                }
            }
        }
    }
}

int odkryj(int x, int y, int n, int m, int plansza[n][m], int odkryte[n][m], int ile){
    if(odkryte[x][y] == 0){
        odkryte[x][y] = 1; // odkrywamy aktualne pole
        ile--; //o jedno mniej zakrytych pol
    }

    if(plansza[x][y] == 0){
        int i, j;
        for(i = x-1; i < x+2; i++){
            for(j = y-1; j < y+2; j++){
                if(i >= 0 && j >= 0 && i < n && j < m){ // jeżeli pole w granicach planszy
                    if(plansza[i][j] == 0 && odkryte[i][j] == 0){
                        ile = odkryj(i, j, n, m, plansza, odkryte, ile);
                    }else if(plansza[i][j] != 9 && odkryte[i][j] == 0){ // jeżeli pole to nie bomba, ale jeszcze nie odkryte, to odkrywam
                        odkryte[i][j] = 1;
                        ile--;
                    }
                }
            }
        }
    }

    return ile;
}

void wypiszPlansze(int n, int m, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int ileNieOdkrytych){
    czysc();
    printf("------SAPER------\n\n");

    int i, j;
    if(m < 36){
        printf("   ");
        for(j = 1; j < m+1; j++) j < 10 ? printf("\033[0;90m%d \033[0m", j) : printf("\033[0;90m%c \033[0m", 'A' + j - 10);
    }
    printf("\n");
    for(i = 0; i < n; i++){
        if(n < 36) i < 9 ? printf("\033[0;90m%d  \033[0m", i+1) : printf("\033[0;90m%c  \033[0m", 'A' + i - 9);
        for(j = 0; j < m; j++){
            if(odkryte[i][j] == 1){
                if(plansza[i][j] == 9){
                    printf("\033[0;31m* \033[0m");
                }else if(plansza[i][j] == 0){
                    printf("\033[0;90m_ \033[0m");
                }else{
                    printf("\033[0;36m%d \033[0m", plansza[i][j]);
                }
            }else if(flagi[i][j] == 1){
                printf("\033[0;35mF \033[0m");
            }else printf("# ");
        }printf("\n");
    }
    printf("\nPozostalo %d pol do odkrycia\n", ileNieOdkrytych);
}

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

int zamien(char znak){
    int x;
    if(znak >= 'A' && znak <= 'Z') x = znak - 'A' + 10;
    else if(znak >= 'a' && znak <= 'z') x = znak - 'a' + 10;
    else if(znak >= '0' && znak <= '9') x = znak - '0';
    return x;
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
                    czySiePowiodlo = 1; // tak
                    if(pierwszePrzejscie == 0){
                        if(plansza[0][0] == -1) stworzPlansze(n,m,p,plansza, iloscBomb, x, y);
                        pierwszePrzejscie++;
                    }
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

void wypiszLeaderboard(FILE *plik){
    int i = 0, punkty;
    char nazwa[20];
    for(; i < 5 && fscanf(plik, "%s ; %d", &nazwa, &punkty) == 2; i++){
        if(i == 0) printf("Tablica wynikow: \n");
        printf("%d. %s  %d\n", i+1, nazwa, punkty);
    }
}

leaderboard* pobierzWyniki(FILE *plik, leaderboard wyniki[5]){
    int i = 0;
    for(; i < 5 && fscanf(plik, "%s ; %d", &wyniki[i].nazwa, &wyniki[i].wynik) == 2; i++){}
    wyniki[0].ilosc = i;
    return wyniki;
}

void dodajWynik(FILE * plik, int wynik, char nazwa[20]){
    leaderboard *wyniki = malloc(5 * sizeof(leaderboard));
    wyniki = pobierzWyniki(plik, wyniki);
    leaderboard temp;
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

int main(int argc, char *argv[]){
    srand(time(NULL));
    FILE *plik = fopen("wyniki.txt", "r");

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