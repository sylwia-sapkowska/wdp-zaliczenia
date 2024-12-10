// Przetwarza zapytania na podstawie testu
// Sylwia Sapkowska 
#include <bits/stdc++.h> 
#include "kol.h"
using namespace std;
typedef pair<int, int> T;

void solve(){
    int m; cin >> m;    
    otwarcie_urzedu(m);
    int n; cin >> n;
    vector<interesant*> v(n);
    for (int i = 0; i < n; i++){
        int x; cin >> x;
        v[i] = nowy_interesant(x);
    }
    int q; cin >> q;
    while (q--){
        string c; cin >> c;
        if (c == "Obsluz"){
            //obsluz
            cerr << "Obsluz" << endl;
            int x; cin >> x;
            interesant *res = obsluz(x);
            cout << (res == nullptr ? -1 : res->id) << endl;
        } 
        if (c == "Naczelnik"){
            //naczelnik
            cerr << "Naczelnik" << endl;
            int x; cin >> x;
            naczelnik(x);
        }
        if (c == "ZamkniecieUrzedu"){
            //zamkniecie
            cerr << "Zamkniecie urzedu" << endl;
            vector<interesant*> res = zamkniecie_urzedu();
            cout << res.size() << endl;
            for (auto x: res){
                cout << x->id << " ";
            }
            cout << endl;
        }
        if (c == "FastTrack"){
            //fast_track
            cerr << "Fast track" << endl;
            int x, y; cin >> x >> y;
            //musza byc w jednej kolejce
            vector<interesant*> res = fast_track(v[x], v[y]);
            cout << res.size() << endl;
            for (auto z: res){
                cout << z->id << " ";
            }
            cout << endl;
        }
        if (c == "ZmianaOkienka"){
            //zmiana okienka
            cerr << "Zmiana okienka" << endl;
            int x, y; cin >> x >> y;
            zmiana_okienka(v[x], y);
        }
        if (c == "Numerek"){
            //numerek
            cerr << "Numerek" << endl;
            int x; cin >> x;
            cout << numerek(v[x]) << endl;
        }
        if (c == "ZamkniecieOkienka"){
            //zamkniecie okienka
            cerr << "Zamkniecie okienka" << endl;
            int x, y; cin >> x >> y;
            zamkniecie_okienka(x, y);
        }
    }
    for (int i = 0; i<n; i++){
        free(v[i]);
    }
}
 
int32_t main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    solve();

    return 0;
}