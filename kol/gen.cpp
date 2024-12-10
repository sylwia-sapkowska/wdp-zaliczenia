//Sylwia Sapkowska 
#include <bits/stdc++.h> 
#pragma GCC optimize("O3", "unroll-loops")
using namespace std;

void __print(int x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << "'" << x << "'";}
void __print(const char *x) {cerr << '"' << x << '"';}
void __print(const string &x) {cerr << '"' << x << '"';}
void __print(bool x) {cerr << (x ? "true" : "false");}

template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? ", " : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifdef LOCAL
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

#define int long long
typedef pair<int, int> T;
const int oo = 1e18, oo2 = 1e9+7, K = 30;
const int mod = 998244353;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int p(int a, int b){
    return a + rng()%(b-a+1);
}

void solve(){
    int ile_interesantów = p(1, 5);
    int ile_okienek = p(1, 5);
    set<int> interesanci;
    cout << ile_okienek << " " << ile_interesantów << "\n";
    vector<vector<int>>a(ile_okienek);
    for (int i = 0; i<ile_interesantów; i++){
        int okienko = p(0, ile_okienek-1);
        a[okienko].push_back(i);
        interesanci.insert(i);
        cout << okienko << " ";
    }
    cout << "\n";
    int q = p(1, 5);
    cout << q+1 << "\n";
    vector<string>opcje = {"Obsluz", "Naczelnik", "ZamkniecieOkienka","Numerek", "FastTrack",  "ZmianaOkienka"};
    while (q--){
        string c = opcje[p(0, interesanci.empty() ? 3 : 5)];
        if (c == "Obsluz"){
            int okienko = p(0, ile_okienek-1);
            cout << c << " " << okienko << "\n";
            if (!a[okienko].empty()) {
                interesanci.erase(a[okienko][0]);
                a[okienko].erase(a[okienko].begin());
            }
        }
        if (c == "Naczelnik"){
            int okienko = p(0, ile_okienek-1);
            cout << c << " " << okienko << "\n";
            reverse(a[okienko].begin(), a[okienko].end());
        }
        if (c == "ZamkniecieOkienka"){
            if (ile_okienek == 1) c = "Numerek";
            else {
                int k1 = p(0, ile_okienek-1);
                int k2 = p(0, ile_okienek-1);
                while (k1 == k2) k2 = p(0, ile_okienek-1);
                cout << c << " " << k1 << " " << k2 << "\n";
                for (auto x: a[k1]){
                    a[k2].push_back(x);
                }
                a[k1].clear();
            }
        }
        if (c == "Numerek"){
            cout << c << " " << p(0, ile_interesantów-1) << "\n";
        }
        if (c == "FastTrack"){
            vector<int>nonempty;
            for (int i = 0; i<ile_okienek; i++){
                if (!a[i].empty()) nonempty.push_back(i);
            }
            assert(nonempty.size() > 0);
            int okienko = nonempty[p(0, nonempty.size()-1)];
            int x = p(0, a[okienko].size()-1);
            int y = p(x, a[okienko].size()-1);
            cout << c << " " << a[okienko][x] << " " << a[okienko][y] << "\n";
            for (int j = x; j<=y; j++){
                interesanci.erase(a[okienko][j]);
            }
            a[okienko].erase(a[okienko].begin() + x, a[okienko].begin() + y + 1);
        }
        if (c == "ZmianaOkienka"){
            vector<int>nonempty;
            for (int i = 0; i<ile_okienek; i++){
                if (!a[i].empty()) nonempty.push_back(i);
            }
            assert(nonempty.size() > 0);
            int okienko = nonempty[p(0, nonempty.size()-1)];
            int osoba = p(0, a[okienko].size()-1);
            int nowe_okienko = p(0, ile_okienek-1);
            cout << c << " " << a[okienko][osoba] << " " << nowe_okienko << "\n";
            a[nowe_okienko].push_back(a[okienko][osoba]);
            a[okienko].erase(a[okienko].begin() + osoba);
        }
    }
    cout << "ZamkniecieUrzedu\n";
}
 
int32_t main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int t = 1;
    //cin >> t;
    while (t--) solve();

    return 0;
}