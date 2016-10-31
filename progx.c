/**
 * "Program x"
 *
 * Program wczytuje ciąg wierszy W1 … Wn reprezentujący instancję problemu dokładnego pokrycia i wypisuje, 
 * w dowolnej kolejności, reprezentacje wszystkich rozwiązań tego problemu.
 *
 * autor: Michal Smutkiewicz
 * wersja: 1.0
 * data: 11 stycznia 2016
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define IND_KONCA_WIERSZA -2
#define IND_KONCA_KOLUMNY -1
#define MIEJSCE_NA_INDYKATOR 2
#define SPACJA ' '
#define NOWY_WIERSZ '\n'

typedef struct
{
    int **tab; /*tablica przechowujaca instancje problemu dokladnego pokrycia*/
    int sizeW; /*rozmiar wierszy tab*/
    int sizeK; /*rozmiar kolumn tab*/
    int capacityW; /*pojemnosc wierszy tab*/
    int capacityK; /*pojemnosc kolumn tab*/
}dyntab;

typedef struct
{
    int **tabind; /*tabind[k][i] zawiera indeksy k kolejnych wierszy niepustych (nie zawierajacych spacji) w odpowiednich kolumnach i*/
    int *drop; /*drop[i] zawiera numery aktualnie rozpatrywanych wierszy z i-tej kolumny*/
}indeksy;

/* Funkcja inicjalizuje tablice dynamiczna tab[][]. 
 * Jako argumenty dostaje strukture zawierajaca podstawowe informacje o tablicy tab[][],
 * poczatkowa pojemnosc wierszy capW, poczatkowa pojemnosc kolumn capK. */
void init(dyntab *t, int capW, int capK)
{
    int i;
    /*capW > 0 && capK > 0*/
    t->tab = (int**)malloc(capW*sizeof(int *));
    for(i = 0; i < capW; i++) t->tab[i] = (int*)malloc(capK*sizeof(int));
        
    t->capacityW = capW;
    t->sizeK = 0;
    t->capacityK = capK;
    t->sizeW = 0;
}

/* Funkcja zwalnia pamiec zaalokowana na tablice uzywane w programie.
 * Zmienna rozmiar ustalamy rozmiar pierwotny tablicy zaalokowany wczesniej.
 */
void clear(dyntab *t, indeksy *p)
{
    int i;
    int rozmiar;
        
    if(t->sizeK > t->sizeW) rozmiar = t->sizeK + MIEJSCE_NA_INDYKATOR;
    else rozmiar = t->sizeW + MIEJSCE_NA_INDYKATOR;

    for(i = 0; i < t->capacityK; i++) free(t->tab[i]);
    free(t->tab);
    t->tab = NULL;
    
    for(i = 0; i < rozmiar; i++) free(p->tabind[i]);
    free(p->tabind);
    p->tabind = NULL;

    free(p->drop);
    p->drop = NULL;
}

/*Funkcja wpisuje do tablicy tab[a][b] wartosc x. Gdy a lub b przekroczy pojemnosc tab, alokowana jest dodatkowa pamiec.*/
void put(dyntab *t, int a, int b, int x)
{
    int i;
    
    if(a >= t->capacityW || b >= t->capacityK)
    {
        t->tab = (int**)realloc(t->tab, 2*(t->capacityK)*sizeof(int *));
        for(i = 0; i < t->capacityW*2; i++) t->tab[i] = (int*)realloc(t->tab[i], 2*t->capacityK*sizeof(int));
        
        t->capacityK *= 2;
        t->capacityW *= 2;

        t->tab[a][b] = x;
        if(a >= t->sizeW) t->sizeW = a; 
        if(b >= t->sizeK) t->sizeK = b;
        /* sizeW (sizeK) jest powiekszana do a (b), wiec maksymalna wielkosc sizeW (sizeK)
         *bedzie rowna indeksowi ostatniego najwiekszego wpisanego elementu*/
    }
    else
    {
        t->tab[a][b] = x;
        if(a >= t->sizeW) t->sizeW = a;
        if(b >= t->sizeK) t->sizeK = b;
    }
    
}

/*Funkcja pobiera kolejne znaki instancji problemu i wpisuje je do tab[][]. 
 *Jako argument dostaje strukture zawierajaca podstawowe informacje o tablicy tab[][]*/
void pobierzplansze(dyntab *t)
{
    int znak; /*przechowuje aktualnie wczytany znak*/
    int i = 0; /*indeks aktualnego wiersza*/
    int j = 0; /*indeks aktualnegj kolumny*/
    do
    {
        do
        {   
            znak = getchar();
            if(znak != EOF)
            {   
                if(znak != NOWY_WIERSZ) put(t, i, j, znak); /*wpisuje znak inny niz enter do tablicy*/
                else put(t, i, j, IND_KONCA_WIERSZA); /*wpisuje -2 ktore traktujemy jak indykator konca wiersza*/
                j++;
            }
            
        }while(znak != NOWY_WIERSZ && znak != EOF);
        
        i++;
        j = 0;
        
    }while(znak != EOF);
}

/*Funkcja wypelnia spacjami tablice tab[][] od ostatniej niepustej komorki tab[][]. 
 * Zastepuje spacjami tymczasowe indykatory konca wiersza. 
 * Zapobiega to zajrzeniu przez nastepne funkcje do komorki pamieci ktora zawiera NULL.
 * Uwaga: funkcja put, by przechowac indykator konca wiersza, ktory jest jednoczesnie indykatorem konca kolumn.
 * rozszerza sizeK do sizeK + 1. 
 * 
 * Przyklad: ("_" to spacja, "I" to indykator "\n")
 * tab przed:    tab po wyrowaniu:
 * \|0123 4       \|0123
 * 0|a__d I       0|a__d
 * 1|_bI          1|_b__
 * 2|__ce I       2|__ce
 
 * sizeK=3+1     sizeK = 3 
 */
void wyrownajplansze(dyntab *t)
{
    int i, j;
    
    for(i = 0; i <= t->sizeW; i++)
    {
       j = 0;
       
        while(j <= t->sizeK && t->tab[i][j] != IND_KONCA_WIERSZA) j++; /*szuka pierwszego indykatora konca wiersza*/
        while(j < t->sizeK) /*wypelnia spacjami puste komorki pamieci az do sizeK*/
        {
            t->tab[i][j] = SPACJA;
            j++;
        }
    }
    t->sizeK -= 1; /*zmniejszamy rozmiar o ostatnia kolumne zawierajaca indykator konca wiersza i kolumn*/

}

/* init_pom_tab() inicjalizuje i alokuje pamiec na pomocnicze tablice programu: tabind[][] i drop[].
 * Jako argument dostaje strukture zawierajaca podstawowe informacje o tablicy tab[][] oraz
 * strukture zawierajaca podstawowe informacje o tablicach pomocniczych tabind[][] i drop[].
 * tabind[k][i] zawiera numery k wierszy niepustych (nie zawierajacych spacji) w odpowiednich kolumnach i.
 Przyklad:
 ("_" to spacja, "K" to indykator konca kolumny)
 
 tab:    tabind:
 \|0123   \| 0 1 2 3
 0|a__d   0| 0 1 2 0
 1|_b__   1| K K K 2
 2|__ce   2|       K
 
 Uwagi: 
 - IND_KONCA_KOLUMNY oznacza dla programu koniec kolumny
 - program dba o to, by nie odczywtywac z tablicy tab z wartosci indykatora konca kolumny np. (tab[-1][kolumna])
 - tabind ma zaalokowana pamiec rozmiaru max(sizeK+1, sizeW+1), size + 1 uzyto by przechowac indykator konca kolumny
 
 drop[i] zawiera numery aktualnie rozpatrywanych wierszy z i-tej kolumny
 Przyklad (dla tablicy wyzej i napisu "abcd"):
 
 drop[]:
 \0123
  0120  to np. tabind[drop[0]][0] = 'a',tabind[drop[1]][1] = 'b' itd.
   */
void init_pom_tab(dyntab *t, indeksy *p)
{
    int i, j, k, rozmiar;
    
    if(t->sizeK > t->sizeW) rozmiar = t->sizeK + MIEJSCE_NA_INDYKATOR;
    else rozmiar = t->sizeW + MIEJSCE_NA_INDYKATOR;
    
    p->tabind = (int**)malloc(rozmiar*sizeof(int *));
    for(i = 0; i < rozmiar; i++) p->tabind[i] = (int*)malloc(rozmiar*sizeof(int));

    p->drop = (int*)calloc(rozmiar, sizeof(int));
    
    for(i = 0; i <= t->sizeK; i++) /*kolejne kolumny*/
    {
        k = 0;/* kolejne wiersze tablicy drop*/
        
        for(j = 0; j <= t->sizeW + 1; j++) /*kolejne wiersze*/
        {
            if(j == t->sizeW + 1) 
            {   
                p->tabind[k][i] = IND_KONCA_KOLUMNY;
            }
            else
            {   
                if(t->tab[j][i] != SPACJA)
                {
                    p->tabind[k][i] = j;
                    k++;
                }
            }
        }
    }
    
}

/*Funkcja zeruje numery numery aktualnie rozpatrywanych wierszy z i-tej kolumny
*Przedzial zerowania jest przekazywany funkcji przez parametry start i koniec.
 *Przyklad (dla tablicy wyzej i napisu "abcd"):
 *Dla start=2 i koniec=3
 *drop[] przed:        drop[] po:
 *\|0|1|2|3            \|0|1|2|3|
 * |0|1|2|2             |0|1|0|0|
 */
void zeruj(indeksy *p, int start, int koniec)
{
    int i;
    for(i = start; i <= koniec; i++) p->drop[i] = 0;
}

/*Funkcja zwraca wartosc true, jesli ciag znakow jest prawidlowym podzbiorem zbioru numerow wierszy (spelniajacym
 *problem dokladnego pokrycia), wpp false. Jako argument dostaje strukture zawierajaca podstawowe informacje o 
 *pomocniczych tablicach tabind i drop. Jako drugi argument dostaje koniec przedzialu numeru kolumn do sprawdzenia.*/
bool CzyPrawidlowe(indeksy *p, int i)
{
    int j, k;
    int a = 0; /*nr aktualnej kolumny*/
    
    while(a <= i)
    {
        for(j=0; p->tabind[j][a] != IND_KONCA_KOLUMNY; j++) /* j to kolejne wiersze w tabind */
        {
            if(j != p->drop[a]) /* pomijamy wiersz aktualnie sprawdzanego znaku */
            {
                for(k=0; k <= i; k++) /* k to kolejne kolumny w tabind */
                {
                    if(k != a) if(p->tabind[j][a] == p->tabind[p->drop[k]][k]) return false;
                    /*porownujemy znak aktualnego wiersza sprawdzanej kolumny z pozostalymi znakami wyrazu*/
                }
            }
        }
        a++;
    }
    return true;
}

/*Funkcja wypisuje prawidlowy napis bedacy rozwiazaniem problemu dokladnego pokrycia.
 * Jako argument dostaje strukture zawierajaca podstawowe informacje o tablicy tab[][] oraz
 * strukture zawierajaca podstawowe informacje o tablicach pomocniczych tabind[][] i drop[].
 * Argument "start" zawiera indeks poczatkowy napisu, a "koniec" koncowy indeks (przedzial domkniety).
 */
void wypisz(dyntab *t, indeksy *p, int start, int koniec)
{
    int i;
    for(i = start; i <= koniec; i++)
    {
        printf("%c", t->tab[p->tabind[p->drop[i]][i]][i]);
    }
    printf("\n");
}

/* Funkcja sprawdza kolejne mozliwe rozwiazania problemu dokladnego pokrycia.
 * Jako argument dostaje strukture zawierajaca podstawowe informacje o tablicy tab[][] oraz
 * strukture zawierajaca podstawowe informacje o tablicach pomocniczych tabind[][] i drop[].
 * 
 * Uwagi i opisy zmiennych:
 * 1. Zmienna "i" sluzy do skakania po kolumnach.
 * 2. Zmienna "CzyZwiekszac" pozwala przejsc algorytmowi do nastepnej kolumny wtw, gdy wie, 
 * ze napis jest prawidlowy do momentu aktualnie sprawdzanej kolumny.
 * 3. Algorytm konczy sie gdy p->tabind[p->drop[0]][0] == IND_KONCA_KOLUMNY 
 * <=> sprawdzilismy wszystkie mozliwosci w zerowej kolumnie, wiec sa to wszystkie mozliwe rozwiazania 
 * <=> p->tabind[p->drop[i]][i] == IND_KONCA_KOLUMNY dla wszystkich i nalezacy do przedzialu <0, t->sizeK>
 */
void sprawdz(dyntab *t, indeksy *p)
{
    int i = 1; /* kolumne zerowa traktujemy szczegolnie, wiec zaczynamy od i=1 */
    bool CzyZwiekszac = false;

    if(t->sizeK > 0 && t->sizeW > 0)
    {
        while(p->tabind[p->drop[0]][0] != IND_KONCA_KOLUMNY)
        {

            if(i + 1 <= t->sizeK && CzyZwiekszac) i++;
            if(p->tabind[p->drop[i]][i] != IND_KONCA_KOLUMNY)
            {
                if(CzyPrawidlowe(p, i))
                {
                    CzyZwiekszac = true;
                    if(i == t->sizeK)
                    {
                        wypisz(t, p, 0, i);
                        p->drop[i]++;
                    }
                }
                else p->drop[i]++;

            }
            else
            {
                zeruj(p, i, t->sizeK);
                if(i >= 1) i--; /* kolumne zerowa traktujemy szczegolnie */
                CzyZwiekszac = false;
            
                if(i > 0) p->drop[i]++;
                else p->drop[i]++; /*i==0*/
            }
        }
    }
}

/*ALGORYTM:
 * 1. Zaninicjalizowanie tablicy glownej przechowujacej instancje problemu, w wymiarach na te chwile, szacunkowych.
 * 2. Pobieranie planszy i inicjalizowania wiekszej ilosc pamieci w razie potrzeby.
 * 3. Wyrownanie planszy i wypelnienie spacjami od pierwszego znaku "\n" w wierszu wlacznie do ostatniej kolumny.
 * 4. Zaninicjalizowanie tablic pomocniczych przechowujacej instancje problemu, w wymiarach ustalonych przez init.
 * 5. Sprawdzanie i wypisywanie rozwiazan.
 * 6. Zwolnienie zaalokowanej pamieci.
 */
int main(void)
{
    dyntab t;
    indeksy p;

    init(&t, 256, 256);
    pobierzplansze(&t);
    wyrownajplansze(&t);
    init_pom_tab(&t, &p);
    sprawdz(&t, &p);
    clear(&t, &p);
	
    return 0;
}
