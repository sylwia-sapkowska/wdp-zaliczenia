// Sylwia Sapkowska

#include "zbior_ary.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

static int Q = -1;

// Funkcja tworzy zbior ciagów o danej liczbie klas ciągów arytmetycznych.
zbior_ary stworz_zbior_ciagow(unsigned liczba_klas){
    zbior_ary nowy_zbior;

    nowy_zbior.ciagi = (klasa_ciagow*)(malloc(liczba_klas * sizeof(klasa_ciagow)));
    assert(nowy_zbior.ciagi != NULL);
    nowy_zbior.liczba_klas = liczba_klas;

    return nowy_zbior;
}

// Funkcja tworzy klase ciagow o danej liczbie ciagow i przypisuje ją do zbioru[klasa]
klasa_ciagow stworz_klase_ciagow(unsigned liczba_ciagow){
    assert(liczba_ciagow > 0);

    klasa_ciagow nowa_klasa;

    nowa_klasa.pocz = (int*)(malloc(liczba_ciagow * sizeof(int)));
    nowa_klasa.kon = (int*)(malloc(liczba_ciagow * sizeof(int)));

    assert(nowa_klasa.pocz != NULL);
    assert(nowa_klasa.kon != NULL);

    // Inicjalizacja początków i końców ciągów jako INT_MAX i INT_MIN
    for (int i = 0; i < (int)liczba_ciagow; i++){
        nowa_klasa.pocz[i] = INT_MAX;
        nowa_klasa.kon[i] = INT_MIN;
    }

    nowa_klasa.liczba_ciagow = liczba_ciagow;
    return nowa_klasa;
}

// Tworzy zbiór ciągów arytmetycznych o jednej klasie zawierającej jeden ciąg arytmetyczny.
zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if (Q == -1){
        Q = q;
    }
    zbior_ary zbior_klas = stworz_zbior_ciagow(1);

    zbior_klas.ciagi[0] = stworz_klase_ciagow(1);

    zbior_klas.ciagi[0].pocz[0] = a;
    zbior_klas.ciagi[0].kon[0] = b;

    return zbior_klas;
}

// Daje w wyniku zbior zlozony z samego elementu a, tj. {a}.
zbior_ary singleton(int a){
    return ciag_arytmetyczny(a, Q, a);
}

int mod(int a){
    int b =  a % Q;
    if (b < 0) {
        b += Q;
    }
    return b;
}

int min(int a, int b){
    return a < b ? a : b;
}

int max(int a, int b){
    return a > b ? a : b;
}

// Sprawdza, czy dwa ciagi tej samej klasy sa rozlaczne
bool rozlaczne(int pocz1, int kon1, int pocz2, int kon2){
    return (kon1 < pocz2 || kon2 < pocz1);
}

// Funkcja dodaje ciąg [pocz, kon] do klasy suma[poz_klasy], sprawdzając czy istnieje możliwość połączenia go z poprzednio rozważonymi ciągami
void dodaj_ciag(klasa_ciagow klasa, int *ostatni, int pocz, int kon){
    if (klasa.pocz[*ostatni] <= klasa.kon[*ostatni] &&  
            rozlaczne(klasa.pocz[*ostatni], klasa.kon[*ostatni], pocz, kon)){
        // Jeśli ciągi arytmetyczne są rozłączne, ale ich suma teoriomnościowa jest jednym ciągiem, to łączymy je w jeden ciąg.
        if ((long long)klasa.kon[*ostatni] + (long long)Q == (long long)pocz){
            klasa.kon[*ostatni] = kon;
            return;
        }
        *ostatni = *ostatni + 1;
        klasa.pocz[*ostatni] = pocz;
        klasa.kon[*ostatni] = kon;
        return;
    }
    // Ciągi nie są rozłączne, więc aktualizujemy początek i koniec sumy ciągów.
    klasa.pocz[*ostatni] = min(klasa.pocz[*ostatni], pocz);
    klasa.kon[*ostatni] = max(klasa.kon[*ostatni], kon);
}

// Funkcja napraw_rozmiar zmienia rozmiar tablicy początków i końców ciągów w zależności od znalezionej ilości rozłącznych ciągów w klasie.
// Algorytmy znajdujące teoriomnogościową sumę, iloczyn i różnicę na początku ich wywołania tworzą klasę ciągów arytmetycznych 
// o maksymalnej możliwej liczbie ciągów, wiec na koniec rozmiar klasy ciagow jest odpowiednio zmniejszany.
void napraw_rozmiar_klasy(klasa_ciagow klasa){
    klasa.pocz = realloc(klasa.pocz, klasa.liczba_ciagow * sizeof(int));
    klasa.kon = realloc(klasa.kon, klasa.liczba_ciagow * sizeof(int));
}

// Łączy dwa zbiory ciągów arytmetycznych tej samej klasy w jedną klasę będącą teoriomnogościową sumą tych zbiorów, 
// Funkcja zapisuje wynik do klasa_sumy.
void polacz(klasa_ciagow *klasa_sumy, klasa_ciagow klasa1, klasa_ciagow klasa2){
    int i = 0, j = 0;
    int rozmiar = 0;
    // Tworzymy nową klasę ciągów arytmetycznych, która będzie miała maksymalnie tyle ciągów, ile suma ciągów w klasach klasa1 i klasa2.
    *klasa_sumy = stworz_klase_ciagow(klasa1.liczba_ciagow + klasa2.liczba_ciagow);
    while (i < (int)klasa1.liczba_ciagow || j < (int)klasa2.liczba_ciagow){
        if ((j == (int)klasa2.liczba_ciagow) || (i < (int)klasa1.liczba_ciagow && klasa1.pocz[i] < klasa2.pocz[j])){
            dodaj_ciag(*klasa_sumy, &rozmiar, klasa1.pocz[i], klasa1.kon[i]);
            i++;
        } else {
            dodaj_ciag(*klasa_sumy, &rozmiar, klasa2.pocz[j], klasa2.kon[j]);
            j++;
        }
    }
    if (klasa_sumy->pocz[0] <= klasa_sumy->kon[0]){
        rozmiar++;
    }
    klasa_sumy->liczba_ciagow = (unsigned)rozmiar;
    napraw_rozmiar_klasy(*klasa_sumy);  
}

// Zwraca resztę klasy, czyli np. resztę z dzielenia początku pierwszego ciągu w klasie przez Q lub INT_MAX jeśli klasa nie istnieje.
int daj_reszte_klasy(zbior_ary *A, int klasa){
    if (klasa >= (int)A->liczba_klas) return INT_MAX;
    return mod(A->ciagi[klasa].pocz[0]);
}

// Daje w wyniku zbior reprezentujacy teoriomnogosciowa sume zbiorow ciągów A i B.
zbior_ary suma(zbior_ary A, zbior_ary B){
    int i = 0, j = 0;
    zbior_ary suma = stworz_zbior_ciagow(A.liczba_klas + B.liczba_klas);
    int rozmiar = 0;
    while (i < (int)A.liczba_klas || j < (int)B.liczba_klas){
        int klasa_i = daj_reszte_klasy(&A, i);
        int klasa_j = daj_reszte_klasy(&B, j);
        
        // Zawsze rozpatrujemy najpierw mniejszą klasę, więc jeśli jeden z indeksów i, j jest większy od liczby klas w 
        // odpowiednio A, B, to daj_reszte_klasy zwróci INT_MAX, zatem ta klasa nigdy nie zostanie wybrana.
        if (klasa_i < klasa_j) { 
            suma.ciagi[rozmiar] = A.ciagi[i];
            i++;
        } else if (klasa_i > klasa_j) {
            suma.ciagi[rozmiar] = B.ciagi[j];
            j++;
        } else {
            polacz(&suma.ciagi[rozmiar], A.ciagi[i], B.ciagi[j]);
            i++;
            j++;   
        }
        rozmiar++;
    }
    suma.liczba_klas = (unsigned)rozmiar;
    suma.ciagi = realloc(suma.ciagi, suma.liczba_klas * sizeof(klasa_ciagow));

    return suma;
}

// Funkcja przetnij_do_klasy dodaje do klasy iloczyn[poz_klasy] ciąg będący przecięciem dwóch ciągów [pocz1, kon1] i [pocz2, kon2].
void przetnij_ciag(klasa_ciagow *klasa, int *ostatni, int pocz, int kon, int pocz2, int kon2){
    if (rozlaczne(pocz, kon, pocz2, kon2)){
        return;
    }
    klasa->pocz[*ostatni] = max(pocz, pocz2);
    klasa->kon[*ostatni] = min(kon, kon2);
    *ostatni = *ostatni + 1;
}

// Znajduje teoriomnogościowy iloczyn dwóch zbiorów ciągów tej samej klasy i zapisuje wynik w klasie iloczyn[poz_klasy].
void przetnij_klasa(klasa_ciagow *klasa_iloczynu, klasa_ciagow klasa1, klasa_ciagow klasa2){
    int i = 0, j = 0;
    int rozmiar = 0;
    *klasa_iloczynu = stworz_klase_ciagow(klasa1.liczba_ciagow + klasa2.liczba_ciagow);
    while (i < (int)klasa1.liczba_ciagow && j < (int)klasa2.liczba_ciagow){
        if (klasa1.kon[i] < klasa2.kon[j]){
            przetnij_ciag(klasa_iloczynu, &rozmiar, klasa1.pocz[i], klasa1.kon[i], klasa2.pocz[j], klasa2.kon[j]);
            i++;
        } else {
            przetnij_ciag(klasa_iloczynu, &rozmiar, klasa2.pocz[j], klasa2.kon[j], klasa1.pocz[i], klasa1.kon[i]);
            j++;
        }
    }
    klasa_iloczynu->liczba_ciagow = (unsigned)rozmiar;
    napraw_rozmiar_klasy(*klasa_iloczynu);
}

// Daje w wyniku zbior reprezentujacy czesc wspolna zbiorow A i B.
zbior_ary iloczyn(zbior_ary A, zbior_ary B){
    int i = 0, j = 0;
    int rozmiar = 0;
    
    zbior_ary iloczyn = stworz_zbior_ciagow(A.liczba_klas + B.liczba_klas);
    while (i < (int)A.liczba_klas && j < (int)B.liczba_klas){
        int klasa_i = daj_reszte_klasy(&A, i);
        int klasa_j = daj_reszte_klasy(&B, j); 
        
        // Jeśli klasa_i różna od klasa_j, to na pewno takie klasy nie znajdują się w przecięciu. 
        if (klasa_i < klasa_j) { 
            i++;
        } else if (klasa_i > klasa_j) {
            j++;
        } else {
            przetnij_klasa(&iloczyn.ciagi[rozmiar], A.ciagi[i], B.ciagi[j]);
            i++;
            j++;
            // Jeśli przecięcie klas A.ciagi[i] i B.ciagi[j] nie było puste, to przesuwamy wskaźnik rozmiar na kolejny indeks klasy.
            if (iloczyn.ciagi[rozmiar].liczba_ciagow > 0) {
                rozmiar++;
            }
        }
    }
    iloczyn.liczba_klas = (unsigned)rozmiar;
    iloczyn.ciagi = realloc(iloczyn.ciagi, iloczyn.liczba_klas * sizeof(klasa_ciagow));
    
    return iloczyn;
}

// Funkcja znajduje teoriomnogościową różnicę dwóch zbiorów ciągów a, b tej samej klasy i zapisuje wynik do klasa_roznicy.
void znajdz_roznice(klasa_ciagow *klasa_roznicy, klasa_ciagow a, klasa_ciagow b){
    *klasa_roznicy = stworz_klase_ciagow(a.liczba_ciagow);
    int j = 0;
    int rozmiar = 0;
    for (int i = 0; i < (int)a.liczba_ciagow; i++){
        // Dla i-tego ciągu należącego do klasy a sprawdzamy wszystkie ciągi z klasy b się z nim przecinające.
        // Utrzymywany jest przedział [l, r] będący podprzedziałem ciągu a[i], który nie został jeszcze rozpatrzony.
        // Gdy napotykamy ciag b[j] przecinający sie z a[i], to dodajemy ciag znajdujacy sie z lewej b[j] nalezacy do a[i]
        // oraz zmieniamy l, by ciąg [l, r] zaczynał się za przedziałem b[j].
        long long l = a.pocz[i];
        long long r = a.kon[i];
        while (j < (int)b.liczba_ciagow && b.pocz[j] <= r) {
            if (b.kon[j] < l) {
                j++;
                continue;
            }
            if (b.pocz[j] > l) {
                klasa_roznicy->pocz[rozmiar] = (int)l;
                klasa_roznicy->kon[rozmiar] = b.pocz[j] - Q;
                rozmiar++;
            } 
            l = (long long)b.kon[j] + (long long)Q;
            if (l > r) break;
            j++;
        }
        if (l <= r){
            klasa_roznicy->pocz[rozmiar] = (int)l;
            klasa_roznicy->kon[rozmiar] = (int)r;
            rozmiar++;
        }
    } 
    klasa_roznicy->liczba_ciagow = (unsigned)rozmiar;
    napraw_rozmiar_klasy(*klasa_roznicy);
}

// Daje w wyniku zbior A \ B.
zbior_ary roznica(zbior_ary A, zbior_ary B){
    int j = 0;
    zbior_ary roznica = stworz_zbior_ciagow(A.liczba_klas);
    int ost = 0;
    for (int i = 0; i < (int)A.liczba_klas; i++){
        while (j < (int)B.liczba_klas && daj_reszte_klasy(&B, j) < daj_reszte_klasy(&A, i)){
            j++;
        }
        if (j < (int)B.liczba_klas && daj_reszte_klasy(&B, j) == daj_reszte_klasy(&A, i)){
            znajdz_roznice(&roznica.ciagi[ost], A.ciagi[i], B.ciagi[j]);
            // Jeśli przecięcie okazało się puste, to nie przesuwamy wskaźnika ost na kolejną klasę.
            if (roznica.ciagi[ost].liczba_ciagow > 0){
                ost++;
            }
        } else {
            roznica.ciagi[ost] = A.ciagi[i];
            ost++;
        }
    }
    roznica.liczba_klas = (unsigned)ost;
    roznica.ciagi = realloc(roznica.ciagi, roznica.liczba_klas * sizeof(klasa_ciagow));

    return roznica;
}

//Sprawdza, czy liczba wartosc należy do przedzialu [a, b].
bool nalezy_do_ciagu(int a, int b, int wartosc){
    if (wartosc < a || wartosc > b) return false;
    return true;
}

// Daje w wyniku true wtw. gdy liczba b nalezy do zbioru A.
bool nalezy(zbior_ary A, int b){
    int reszta = mod(b);

    // Ponieważ klasy są posortowane względem reszty elementów mod Q, to możemy zastosować wyszukiwanie binarne po wyniku
    // by znaleźć klasę mającą resztę reszta.
    int l = 0, r = (int)A.liczba_klas - 1;
    int poz_klasy = -1; 
    
    while (r >= l){
        int srodek = (l + r) / 2; 
        if (daj_reszte_klasy(&A, srodek) <= reszta) {
            poz_klasy = srodek;
            l = srodek + 1;
        } else {
            r = srodek - 1;
        }
    }
    if (poz_klasy == -1 || daj_reszte_klasy(&A, poz_klasy) != reszta) return false;

    // Ponieważ ciągi w klasie są posortowane względem początków, to również tu możemy zastosować wyszukiwanie binarne po wyniku, 
    // żeby znaleźć ciąg zawierający liczbę b.
    int l2 = 0, r2 = (int)A.ciagi[poz_klasy].liczba_ciagow - 1;
    while (r2 >= l2){
        int srodek = (l2 + r2) / 2;
        if (nalezy_do_ciagu(A.ciagi[poz_klasy].pocz[srodek], A.ciagi[poz_klasy].kon[srodek], b)){
            return true;
        }
        if (A.ciagi[poz_klasy].pocz[srodek] > b){
            r2 = srodek - 1;
        } else {
            l2 = srodek + 1;
        }
    }

    return false;
}

// Wynikiem funkcji jest liczba elementow w zbiorze A.
unsigned moc(zbior_ary A){
    unsigned liczba_elementow = 0;
    // Liczymy liczbę elementów w każdym ciągu arytmetycznym w klasie.
    for (int i = 0; i < (int)A.liczba_klas; i++){
        for (int j = 0; j < (int)A.ciagi[i].liczba_ciagow; j++){
            long long dlugosc_ciagu_arytm = ((long long)A.ciagi[i].kon[j] - (long long)A.ciagi[i].pocz[j]) / (long long)Q + 1;
            assert(dlugosc_ciagu_arytm > 0);
            liczba_elementow += (unsigned)dlugosc_ciagu_arytm;
        }
    }
    return liczba_elementow;
}

// Funkcja zwraca Ary_Q(A)
unsigned ary(zbior_ary A){
    unsigned ary_q = 0;
    // W każdej klasie i jest A.ciag[i].liczba_ciagow rozłącznych ciągów arytmetycznych.
    for (int i = 0; i < (int)A.liczba_klas; i++){
        ary_q += A.ciagi[i].liczba_ciagow;
    }
    return ary_q;
}
