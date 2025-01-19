#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

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
            plansza[x][y] = iloscBomb+1; // bomba
            i++;
        }
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(plansza[i][j] == iloscBomb+1){ // jeżeli jesteśmy na polu bomby
                for(x = i-1; x < i+2; x++){
                    for(y = j-1; y < j+2; y++){
                        if(x >= 0 && y >= 0 && x < n && y < m && plansza[x][y] != iloscBomb+1){ // jeżeli pole w granicach planszy i nie jest bomba
                            plansza[x][y]++; // zwiekszamy nr pola (ilosc otaczających je bomb)
                        }
                    }
                }
            }
        }
    }
}

int odkryj(int x, int y, int n, int m, int plansza[n][m], int odkryte[n][m], int ile, int ileBomb){
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
                        ile = odkryj(i, j, n, m, plansza, odkryte, ile, ileBomb);
                    }else if(plansza[i][j] != ileBomb+1 && odkryte[i][j] == 0){
                        odkryte[i][j] = 1;
                        ile--;
                    }
                }
            }
        }
    }

    return ile;
}

int ileCyfr(int x, char komenda[50]){
    while(komenda[x] != ' ' && komenda[x] != '\n'){
        x++;
    }
    return x;
}

int pobierzLiczbe(int l, char komenda[50], char liczba[50], int iLiczby){
    int o = 0;
    for(; l<iLiczby; l++){
        liczba[o] = komenda[l];
        o++;
    }
    int zwroc;
    if(liczba[0] >= 'A' && liczba[0] <= 'Z'){
        zwroc = 10 + liczba[0] - 'A';
    }else if(liczba[0] >= 'a' && liczba[0] <= 'z'){
        zwroc = 10 + liczba[0] - 'a';
    }else{
        zwroc = atoi(liczba);
    }
    return zwroc;
}

void wypiszPlansze(int n, int m, int plansza[n][m], int odkryte[n][m], int flagi[n][m], int iloscBomb, int ileNieOdkrytych){
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
                if(plansza[i][j] == iloscBomb + 1){
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

int main(){
    srand(time(NULL));

    int i, j;
    czysc();

    printf("------SAPER------\n\n");
    int n,m, t; // n x m - rozmiary planszy, p - prawdopodobienstwo wystapienia bomby na danym polu, t - maksymalna ilosc czasu w minutach
    double p;
    int mnoznik;
    int stop = 1;

    while(stop){
        printf("Wybierz poziom trudności z dostępnych:\n1. Latwy\n2. Sredni\n3. Trudny\n4. Wlasna plansza\n\n");
        char wybor[20];
        printf("Prosze wpisac nazwe poziomu trudnosci lub liczbe jej odpowiadajaca: ");
        fgets(wybor, 20, stdin);
        wybor[strcspn(wybor, "\n")] = '\0'; // Usuwa znak znak konca linii z pobranego tekstu

        if(stricmp(wybor, "latwy") == 0 || stricmp(wybor, "1") == 0 || stricmp(wybor, "1.") == 0){
            n=8, m=8, p=0.1, mnoznik = 1, t = 10;
            stop = 0;
        }else if(stricmp(wybor, "sredni") == 0 || stricmp(wybor, "2") == 0 || stricmp(wybor, "2.") == 0){
            n=16,m=16, p=0.15, mnoznik = 2, t = 40;
            stop = 0;
        }else if(stricmp(wybor, "trudny") == 0 || stricmp(wybor, "3") == 0 || stricmp(wybor, "3.") == 0){
            n=16,m=32, p=0.2, mnoznik = 3, t = 99;
            stop = 0;
        }else if(stricmp(wybor, "wlasna plansza") == 0 || stricmp(wybor, "4") == 0 || stricmp(wybor, "4.") == 0){
            printf("Proszę podac wielkosc planszy w formacie axb gdzie a to liczba wierszy a b to liczba kolumn: ");
            if(scanf("%dx%d", &n, &m) == 2) stop = 2; // przechodzi zeby pobrac limit czasu
            else{ // pobieranie od poczatku
                czysc();
                printf("Podano rozmiary planszy w zlym formacie. Prosze zapisac je w formacie axb, gdzie a to liczba wierszy a b to liczba kolumn\n\n");
                while (getchar() != '\n'); // czyszczenie bufora
            }
            
            if(stop == 2){
                printf("Proszę podac limit czasu na gre: ");
                if(scanf("%d", &t) == 1) stop = 0; // pobrano wszystkie dane, mozna wyjsc z petli
                else{ // pobieranie od poczatku
                    czysc();
                    printf("Podano limit czasu w zlym formacie. Prosze podac liczbe calkowita\n\n");
                    while (getchar() != '\n'); // czyszczenie bufora
                }
            }
            p=0.2, mnoznik = 2;
        }else{
            czysc();
            printf("Podano zla nazwe poziomu gry. Prosze podac jedna z dostepnych (liczbe lub nazwe trudnosci)\n\n");
        }
    }

    int start = time(NULL);

    czysc();
    printf("------SAPER------\n\n");

    int plansza[n][m];
    int odkryte[n][m]; // odkryte = 1, nie odtyte = 0
    int flagi[n][m]; // flaga = 1, brak flagi = 0
    wypelnij0(n, m, odkryte);
    wypelnij0(n,m,flagi);

    int iloscBomb = n*m*p;
    if(rand()%2 == 0) iloscBomb++;

    int ileNieOdkrytych = n*m - iloscBomb, ileNieOdkrytychPol = ileNieOdkrytych;
    int pierwszePrzejscie = 0;

    char komenda[50], liczba[50];
    int iL1, iL2, l1, l2;
    int czySiePowiodlo;

    wypiszPlansze(n, m, plansza, odkryte, flagi, iloscBomb, ileNieOdkrytych);

    stop = 1;
    while(ileNieOdkrytych > 0 && stop == 1){
        czySiePowiodlo = 0; // nie
        printf("> ");
        fgets(komenda, sizeof(komenda), stdin);

        if(komenda[1] != ' ' || (komenda[0] != 'f' && komenda[0] != 'r')) printf("Podano zla formule komendy. Komenda powinna wygladac nastepujaco:\nf/r x y\ngdzie f/r to wybor czy odkrywamy pole(r) czy stawiamy flage(f), a x oraz y to wspolczynniki pola, do ktorego ma sie odnosic akcja.\n");
        else{
            iL1 = ileCyfr(2, komenda);
            memset(liczba, '\0', sizeof(liczba)); // Ustawia wszystkie elementy na '\0'
            l1 = pobierzLiczbe(2, komenda, liczba, iL1);

            iL2 = ileCyfr(iL1+1, komenda);
            memset(liczba, '\0', sizeof(liczba)); // Ustawia wszystkie elementy na '\0'
            l2 = pobierzLiczbe(iL1 + 1, komenda, liczba, iL2);

            if(l1 < 1 || l2 < 1 || l1 > n || l2 > m) printf("Podane koordynaty musza byc dodatnie oraz nie wieksze od rozmiarow planszy\n");
            else{
                l1--;
                l2--;
                if(komenda[0] == 'f'){
                    if(pierwszePrzejscie == 0) printf("Przed stawianiem flag odkryj pierwsze pole\n");
                    else{
                        if(flagi[l1][l2] == 0) flagi[l1][l2] = 1; // stawiamy flage w danym miejscu
                        else flagi[l1][l2] = 0; // usuwamy flage z pola
                        czySiePowiodlo = 1; //tak
                    }
                }else if(komenda[0] == 'r'){
                    czySiePowiodlo = 1; // tak
                    if(pierwszePrzejscie == 0){
                        stworzPlansze(n,m,p,plansza, iloscBomb, l1, l2);
                        pierwszePrzejscie++;
                    }
                    if(flagi[l1][l2] == 1){ // jeżeli na danym polu jest flaga, usuwa flage, ale nie odkrywa pola
                        flagi[l1][l2] = 0;
                    }else{
                        odkryte[l1][l2] = 1;
                        ileNieOdkrytych--;
                        if(plansza[l1][l2] == iloscBomb+1){
                            ileNieOdkrytych++; //bomba nie zalicza sie do odkrytych pol
                            stop = 0;
                        }else ileNieOdkrytych = odkryj(l1, l2, n, m, plansza, odkryte, ileNieOdkrytych, iloscBomb);
                    }
                }else{
                    printf("Zle podany pierwszy argument. Dostepne opcje:\n r - odkrywanie pola\n f - stawianie flagi\nPodaj poprawna komende.\n");
                }
            }
        }
        if(czySiePowiodlo == 1){
            wypiszPlansze(n, m, plansza, odkryte, flagi, iloscBomb, ileNieOdkrytych);
            printf("Aktualny wynik: %d\n", mnoznik * (ileNieOdkrytychPol - ileNieOdkrytych));
            double aktualny_czas = wypiszCzas(start);
            if (aktualny_czas > t){
                stop = 0;
                printf("Koniec czasu! ");
            }
        }
    }
    if(ileNieOdkrytych == 0){
        printf("Wygrales!!! Ilosc zdobytych punktow: %d\n", mnoznik * (ileNieOdkrytychPol - ileNieOdkrytych));
    }else{
        printf("Przegrales!\n");
    }
}

// 0 - puste, liczba - liczba do wypisania (ilosc bomb naokolo), ileBomb+1 - bomba