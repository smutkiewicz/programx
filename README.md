# programx
Problem dokładnego pokrycia

 * "Program x"
 *
 * Program wczytuje ciąg wierszy W1 … Wn reprezentujący instancję problemu dokładnego pokrycia i wypisuje, 
 * w dowolnej kolejności, reprezentacje wszystkich rozwiązań tego problemu.
 *
 * autor: Michal Smutkiewicz
 * wersja: 1.0
 * data: 11 stycznia 2016

Wprowadzenie

Rozwiązaniem problemu dokładnego pokrycia (ang. exact cover) dla rodziny P podzbiorów zbioru S jest podzbiór Q zbioru P taki, że każdy element S należy do dokładnie jednego elementu Q.
Problem dokładnego pokrycia jest NP-zupełny. Nie jest znany żaden deterministyczny algorytm, który rozwiązuje go w czasie wielomianowym. Algorytm o koszcie wykładniczym, oparty na metodzie prób i błędów, jest przez Donalda Knutha nazywany Algorytmem X.

Program x, wczytuje ciąg wierszy W1 … Wn reprezentujący instancję problemu dokładnego pokrycia i wypisze, w dowolnej kolejności, reprezentacje wszystkich rozwiązań tego problemu.

Rozwiązaniem problemu jest podzbiór Q zbioru numerów wierszy {1 … n}. Reprezentacją rozwiązania jest wiersz R spełniający wszystkie poniższe warunki:
•	długość R jest równa maksimum z długości W1 … Wn,
•	w R nie występuje żadna spacja,
•	dla każdego znaku Ri kolumny i-tej wiersza R istnieje element j zbioru Q taki, że (Wj)i = Ri i dla każdego k w zbiorze Q różnego od j, (Wk)i = ' '.

Program wybiera znaki wierszy wynikowych metodą prób i błędów, w kolejności od początku wiersza, uwzględniając podjęte wcześniej decyzje.
Najpierw więc, na wszystkie możliwe sposoby, wybieramy znak, który znajdzie się w pierwszej kolumnie wiersza wynikowego. Następnie, również na wszystkie możliwe sposoby, wybieramy do kolumny drugiej znak nie kolidujący z wyborem dokonanym w kolumnie pierwszej itd. dla wszystkich pozostałych kolumn.

Przykłady
Dla danych:
a  b
 c
d e
    f
  g
 h i
j   k

wynik programu, po posortowaniu, ma postać:

acgbf
dheif
jhgik

Dla danych:
a  b
 c d  e
f
  gh
    ij k
l     m
 n   o
  p
      q
     r s
t
 uv
    w
 x   yz
 
wynik programu, po posortowaniu, ma postać:

auvbijqk
auvbwrqs
fcpdijek
fcpdwres
tcpdijek
tcpdwres

Uwagi

Zakładamy, że w każdym wierszu danych wejściowych jest co najmniej jeden znak różny od spacji.
Zakładamy, że długość wiersza danych wejściowych nie przekroczy zakresu typu int.
