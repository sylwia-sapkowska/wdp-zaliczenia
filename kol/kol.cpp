#include <vector>
#include <cstdlib>
#include <cassert>
#include "kol.h"
using namespace std;

namespace {
    // Struktura kolejki przy pomocy listy dwukierunkowej
    struct kolejka{
        interesant *przod, *tyl;
    };

    // Tworzy nową kolejkę z dwoma wartownikami
    void stworz_kolejke(kolejka*q){
        q->przod = (interesant*)malloc(sizeof(interesant));
        q->tyl = (interesant*)malloc(sizeof(interesant));
        q->przod->prawy = q->tyl;
        q->tyl->prawy = q->przod;
        q->przod->lewy = q->przod;
        q->tyl->lewy = q->tyl;
        q->przod->id = -1;
        q->tyl->id = -1;
    }

    // Sprawdza czy kolejka jest pusta
    bool pusta(kolejka*q){
        return q->przod->prawy == q->tyl;
    }

    void polacz(interesant* el, interesant *poprz, interesant *nowy){
        if (el){
            if (el->lewy == poprz) el->lewy = nowy;
            else el->prawy = nowy;
        }
    }

    // Wstawia nowy element pomiędzy p i q
    void wsadz_pomiedzy(interesant *p, interesant *q, interesant *r){
        r->lewy = p;
        r->prawy = q;
        polacz(p, q, r);
        polacz(q, p, r);
    }

    // Dodaje element na koniec kolejki
    void dodaj_na_tyl(kolejka *q, interesant *el){
        wsadz_pomiedzy(q->tyl, q->tyl->prawy, el);
    }

    // Usuwa element z kolejki
    void usun_interesanta(interesant *e){
        assert(e->id != -1);
        auto e1 = e->lewy;
        auto e2 = e->prawy;
        polacz(e1, e, e2);
        polacz(e2, e, e1);
    }

    // Usuwa element z przodu kolejki i zwraca wskaźnik na niego
    interesant* usun_z_przodu(kolejka *q){
        if (pusta(q)) return nullptr;
        interesant* i = q->przod->prawy;
        usun_interesanta(q->przod->prawy);
        return i;
    }

    // Łączy dwie kolejki
    void polacz_kolejki(kolejka *l1, kolejka *l2){
        interesant* f1 = l1->przod;
        interesant* b1 = l1->tyl;
        interesant* f2 = l2->przod;
        interesant* b2 = l2->tyl;

        interesant* e1 = b1->prawy;
        interesant* e2 = f2->prawy;
        assert(f1 != b1 && f2 != b2);
    
        polacz(e1, b1, e2);
        polacz(e2, f2, e1);
        l1->tyl = b2;
        f2->prawy = b1;
        b1->prawy = f2;
        l2->tyl = b1;
    }
}

// Operacje urzedowe - zgodnie z opisem w kol.h

// Dane: kolejki okienek, liczba okienek oraz liczba interesantów, którzy do tej pory przyszli do urzędu
vector<kolejka*> okienka;
static int liczba_okienek;
int numerki = 0;

void otwarcie_urzedu(int m){
    liczba_okienek = m;
    okienka.resize(m);
    for (int i = 0; i < m; i++){
        okienka[i] = (kolejka*)malloc(sizeof(kolejka));
        stworz_kolejke(okienka[i]);
    }
}

interesant *nowy_interesant(int k){
    interesant *nowy = (interesant*)malloc(sizeof(interesant));
    nowy->id = numerki++;
    dodaj_na_tyl(okienka[k], nowy);
    return nowy;    
}

int numerek(interesant *i){
    return i->id;
}

interesant *obsluz(int k){
    return usun_z_przodu(okienka[k]);
}

void zmiana_okienka(interesant *i, int k){
    usun_interesanta(i);
    dodaj_na_tyl(okienka[k], i);
}

void zamkniecie_okienka(int k1, int k2){
    assert(k1 != k2);
    polacz_kolejki(okienka[k2], okienka[k1]);
}

void naczelnik(int k){
    kolejka* q = okienka[k];
    interesant *tmp = q->przod;
    q->przod = q->tyl;
    q->tyl = tmp;
}

vector<interesant*> fast_track(interesant *i1, interesant *i2){
    if (i1 == i2){
        usun_interesanta(i1);
        return vector<interesant*>{i1};
    }
    interesant *lewo = i1->lewy, *prawo = i1->prawy;
    interesant *lewo_nast = i1, *prawo_lewy = i1;
    // Idea: zaczynamy od i1, idziemy w lewo i w prawo jednocześnie, aż dojdziemy do i2
    // wektory interesantów kolejno z lewej i z prawej strony
    vector<interesant*> inter_lewo  = vector<interesant*>{i1};
    vector<interesant*> inter_prawo = vector<interesant*>{i1};
    while (lewo != i2 && prawo != i2){
        // Jeśli lewo ma id równe -1, to znaczy, że trafiliśmy na początek lub koniec kolejki
        // więc nie możemy iść dalej
        if (lewo->id != -1) {
            inter_lewo.push_back(lewo);
            interesant*tmp = lewo;
            if (lewo->lewy == lewo_nast) lewo = lewo->prawy;
            else lewo = lewo->lewy;
            lewo_nast = tmp;
        }
        if (prawo->id != -1) {
            inter_prawo.push_back(prawo);
            interesant*tmp = prawo;
            if (prawo->prawy == prawo_lewy) prawo = prawo->lewy;
            else prawo = prawo->prawy;
            prawo_lewy = tmp;
        } 
    }
    inter_lewo.push_back(i2);
    inter_prawo.push_back(i2);
    if (lewo != i2) inter_lewo.swap(inter_prawo);
    for (auto x : inter_lewo) usun_interesanta(x);
    return inter_lewo;
}

vector<interesant*> zamkniecie_urzedu(){
    vector<interesant*> pozostali;
    for (int i = 0; i < liczba_okienek; i++){
        while (!pusta(okienka[i])){
            pozostali.push_back(usun_z_przodu(okienka[i]));
        }
        assert(okienka[i]->przod->prawy == okienka[i]->tyl);
        // Zwolnienie pamięci
        free(okienka[i]->przod);
        free(okienka[i]->tyl);
        free(okienka[i]);
    }
    return pozostali;
}