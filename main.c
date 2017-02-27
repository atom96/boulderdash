#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#define MAX_WIERSZY 100
#define MAX_KOLUMN 200

#define ROCKFORD '&'
#define PUSTE ' '
#define ZIEMIA '.'
#define KAMIEN 'O'
#define DIAMENT '$'
#define WYJSCIE 'X'

#define GORA 'w'
#define DOL 's'
#define LEWO 'a'
#define PRAWO 'd'

#define KONIEC_WIERSZA '\n'
#define KONIEC "Wygrales!!!\n"

void wczytajPlansze(char plansza[][MAX_KOLUMN], int *ilWierszy, int *ilKolumn,
                    int *ilDiamentow, int *wRockford, int *kRockford);

void wypiszPlansze(char plansza[][MAX_KOLUMN], int ilWierszy, int ilKolumn);

void stabilizujPlansze(char plansza[][MAX_KOLUMN], int ilWierszy,
                       int ilKolumn);

void stabilizujKolumne(char plansza[][MAX_KOLUMN], int ilWierszy, int k);

void zamien(char *x, char *y);

void strona(int polecenie, int wRockford, int kRockford, int *wDocelowe,
            int *kDocelowe);

void ruch(char plansza[][MAX_KOLUMN], int ilWierszy, int *wRockford,
          int *kRockford, int polecenie, int ilDiamentow, int *zebDiamenty,
          bool *zakoncz);

void idz(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
         int wDocelowe, int kDocelowe, int ilWierszy);

void idzKamien(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
               int wDocelowe, int kDocelowe, int ilWierszy, int str);

void idzWyjscie(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
                int ilWierszy, bool *zakoncz);

void graj(void);

#ifdef TUI
void grajTUI(void);
void wypiszPlanszeTUI (char plansza[][MAX_KOLUMN], int ilWierszy);
void inicjuj_tui(void);
#endif

int main(void) {
#ifdef TUI
    grajTUI();
#else
    graj();
#endif
    return 0;
}

/**
 * Funkcja ta wczytuje plansze do tablicy dwuwymiarowej, liczy ilość diamentów
 * na planszy i zapisuje położenie Rockforda.
 */
void wczytajPlansze(char plansza[][MAX_KOLUMN], int *ilWierszy, int *ilKolumn,
                    int *ilDiamentow, int *wRockford, int *kRockford) {
    int akt;
    int ilEnter = 0;
    int w = 0;
    int k = 0;

    while (ilEnter < 2 && (akt = getchar()) != EOF) {
        if (akt != KONIEC_WIERSZA) {
            plansza[w][k] = akt;
            if (akt == DIAMENT) {
                ++*ilDiamentow;
            } else if (akt == ROCKFORD) {
                *wRockford = w;
                *kRockford = k;
            }
            ++k;
            ilEnter = 0;
        } else {
            if (w == 0) *ilKolumn = k;
            ++w;
            ++ilEnter;
            k = 0;
        }
    }

    *ilWierszy = w - 1;
}

/**
 * Funkcja ta wypisuje plansze
 */
void wypiszPlansze(char plansza[][MAX_KOLUMN], int ilWierszy,
                   int ilKolumn) {
    int w;
    int k;
    for (w = 0; w < ilWierszy; ++w) {
        for (k = 0; k < ilKolumn; ++k) {
            putchar(plansza[w][k]);
        }
        putchar(KONIEC_WIERSZA);
    }
}

/**
 * Funkcja ta stabilizuje całą planszę
 */
void
stabilizujPlansze(char plansza[][MAX_KOLUMN], int ilWierszy, int ilKolumn) {
    int k;
    for (k = 1; k < ilKolumn - 1; ++k) {
        stabilizujKolumne(plansza, ilWierszy, k);
    }
}

/**
 * Funkcja ta stabilizuje określoną kolumnę
 */
void stabilizujKolumne(char plansza[][MAX_KOLUMN], int ilWierszy, int k) {
    int w = ilWierszy - 2;
    int ilePuste = 0;
    int ostPuste;
    while (w > 0) {
        if (plansza[w][k] == PUSTE) {
            if (ilePuste == 0) {
                ostPuste = w;
            }
            ++ilePuste;
        } else if ((plansza[w][k] == KAMIEN || plansza[w][k] == DIAMENT)
                   && (ilePuste > 0)) {
            zamien(&plansza[w][k], &plansza[ostPuste][k]);
            --ostPuste;
        } else {
            ilePuste = 0;
        }
        --w;
    }
}

/**
 * Funkcja ta zamienia dwie zmienne
 */
void zamien(char *x, char *y) {
    char pom;
    pom = *x;
    *x = *y;
    *y = pom;
}

/**
 * Funkcja ta rozpoznaje w jaką stronę ma ruszyć się Rockford
 */
void strona(int polecenie, int wRockford, int kRockford, int *wDocelowe,
            int *kDocelowe) {
    switch (polecenie) {
        case PRAWO:
            *wDocelowe = wRockford;
            *kDocelowe = kRockford + 1;
            break;
        case LEWO:
            *wDocelowe = wRockford;
            *kDocelowe = kRockford - 1;
            break;
        case GORA:
            *wDocelowe = wRockford - 1;
            *kDocelowe = kRockford;
            break;
        case DOL:
            *wDocelowe = wRockford + 1;
            *kDocelowe = kRockford;
            break;
        default:
            *wDocelowe = wRockford;
            *kDocelowe = kRockford;
    }

}

/**
 * Funkcja ta mówi jak ma się ruszyć Rockford w zależności od pola,
 * na jakie chce wejść
 */
void ruch(char plansza[][MAX_KOLUMN], int ilWierszy, int *wRockford,
          int *kRockford, int polecenie, int ilDiamentow, int *zebDiamenty,
          bool *zakoncz) {
    int kDocelowe;
    int wDocelowe;
    strona(polecenie, *wRockford, *kRockford, &wDocelowe, &kDocelowe);
    switch (plansza[wDocelowe][kDocelowe]) {
        case ROCKFORD:
            break;
        case PUSTE:
            idz(plansza, wRockford, kRockford, wDocelowe, kDocelowe, ilWierszy);
            break;
        case ZIEMIA:
            idz(plansza, wRockford, kRockford, wDocelowe, kDocelowe, ilWierszy);
            break;
        case DIAMENT:
            idz(plansza, wRockford, kRockford, wDocelowe, kDocelowe, ilWierszy);
            ++*zebDiamenty;
            break;
        case WYJSCIE:
            if (*zebDiamenty == ilDiamentow) {
                idzWyjscie(plansza, wRockford, kRockford, ilWierszy, zakoncz);
            }
            break;
        case KAMIEN:
            if (polecenie == PRAWO) {
                idzKamien(plansza, wRockford, kRockford, wDocelowe, kDocelowe,
                          ilWierszy, 1);
            } else if (polecenie == LEWO) {
                idzKamien(plansza, wRockford, kRockford, wDocelowe, kDocelowe,
                          ilWierszy, -1);
            }
            break;
    }

}

/**
 * Ta funkcja realizuje przejście Rockforda na docelowe pole 
 */
void idz(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
         int wDocelowe, int kDocelowe, int ilWierszy) {
    plansza[wDocelowe][kDocelowe] = ROCKFORD;
    plansza[*wRockford][*kRockford] = PUSTE;
    stabilizujKolumne(plansza, ilWierszy, *kRockford);
    *kRockford = kDocelowe;
    *wRockford = wDocelowe;
}

/**
 * Ta funkcja realizuje przejście Rockforda, jeże chce on wejść na pole 
 * z wyjściem
 */
void idzWyjscie(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
                int ilWierszy, bool *zakoncz) {
    plansza[*wRockford][*kRockford] = PUSTE;
    stabilizujKolumne(plansza, ilWierszy, *kRockford);
    *zakoncz = true;
}

/**
 * Ta funkcja odpowiada za ruch Rockforda, jeżeli chce wejść on 
 * na pole z kamieniem
 */
void idzKamien(char plansza[][MAX_KOLUMN], int *wRockford, int *kRockford,
               int wDocelowe, int kDocelowe, int ilWierszy, int str) {
    if (plansza[wDocelowe][kDocelowe + str] == PUSTE) {
        idz(plansza, wRockford, kRockford, wDocelowe, kDocelowe, ilWierszy);
        plansza[wDocelowe][kDocelowe + str] = KAMIEN;
        stabilizujKolumne(plansza, ilWierszy, kDocelowe + str);
    }
}

/**
 * Ta funkcja odpowiada za przeprowadzenie rozgrywki w domyślnym trybie
 * (bez TUI)
 */

void graj(void) {
    char plansza[MAX_WIERSZY][MAX_KOLUMN];
    int ilKolumn = 0;
    int ilWierszy = 0;
    int kRockford;
    int wRockford;
    int ilDiamentow = 0;
    int zebDiamenty = 0;
    int akt;
    bool zakoncz = false;
    wczytajPlansze(plansza, &ilWierszy, &ilKolumn, &ilDiamentow, &wRockford,
                   &kRockford);
    stabilizujPlansze(plansza, ilWierszy, ilKolumn);
    while (!zakoncz && (akt = getchar()) != EOF) {
        ruch(plansza, ilWierszy, &wRockford, &kRockford, akt, ilDiamentow,
             &zebDiamenty, &zakoncz);
    }
    wypiszPlansze(plansza, ilWierszy, ilKolumn);
}

#ifdef TUI
/**
 * Ta funkcja odpowiada za przeprowadzenie rozgrywki w trybie TUI
 */
void grajTUI(void)
{
    char plansza[MAX_WIERSZY][MAX_KOLUMN]; 
    int ilKolumn=0; 
    int ilWierszy=0;
    int kRockford;
    int wRockford;
    int ilDiamentow=0;
    int zebDiamenty=0;
    int akt; 
    bool zakoncz=false;

    wczytajPlansze(plansza, &ilWierszy,  &ilKolumn, &ilDiamentow, &wRockford,
            &kRockford);
    stabilizujPlansze(plansza, ilWierszy, ilKolumn);
    inicjuj_tui();
    wypiszPlanszeTUI(plansza,ilWierszy);

    while (!zakoncz&&(akt=getchar())!=EOF){
        ruch(plansza,ilWierszy,&wRockford,&kRockford,akt,ilDiamentow,
                &zebDiamenty,&zakoncz);
        wypiszPlanszeTUI(plansza,ilWierszy);
    }

    if(zakoncz){
    mvprintw(ilWierszy,0,KONIEC);
    }

    refresh();
    getchar();
    endwin();
}
/**
 * Ta funkcja wypisuje planszę na ekran w trybie TUI
 */
void wypiszPlanszeTUI (char plansza[][MAX_KOLUMN], int ilWierszy)
{
    int i;
    for(i=0;i<ilWierszy;++i){
        mvprintw(i,0,"%s",plansza[i]);
    }
    refresh();
}
/**
 * Funkcja inicjująca TUI
 */
void inicjuj_tui(void)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}
#endif
