//Sylwia Sapkowska 
#include <bits/stdc++.h> 
#pragma GCC optimize("O3", "unroll-loops")
using namespace std;


mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int losuj(int a, int b){
    return a + rng()%(b-a+1);
}

pair<int, int>para(int M, int Q){
    int b = losuj(INT_MAX-M, INT_MAX);
    return {b - losuj(0, M) * Q, b};
}
const int M = 10;

void solve(){
    int op = losuj(1, M);
    cout << op+1 << "\n";
    int cnt = 1;
    
    int Q =  losuj(1, 10);
    auto [A, B] = para(M, Q);
    cout << "C " << A << " " << Q << " " << B << "\n";

    for (int i = 0; i<op; i++){
        int curr = losuj(1, 8);
        auto [a, b] = para(M, Q);
        switch (curr)
        {
        case 1: //ciag arytmetyczny
            cout << "C " << a << " " << Q << " " << b << "\n";
            cnt++;
            break;
        case 2: //singleton
            cout << "S " << losuj(INT_MAX-M, INT_MAX) << "\n";
            cnt++;
            break;

        case 3: //suma
            cout << "U " << losuj(1, cnt) << " " << losuj(1, cnt) << "\n";
            cnt++;
            break;

        case 4: //iloczyn
            cout << "I " << losuj(1, cnt) << " " << losuj(1, cnt) << "\n";
            cnt++;
            break;

        case 5: //roznica
            cout << "R " << losuj(1, cnt) << " " << losuj(1, cnt) << "\n";
            cnt++;
            break;

        case 6: //nalezy
            cout << "N " << losuj(1, cnt) << " " << losuj(1, M) << "\n";
            break;
        
        case 7: //moc  
            cout << "M " << losuj(1, cnt) << "\n";
            break;
        
        case 8: //ary
            cout << "A " << losuj(1, cnt) << "\n";
            break;

        default:
            break;
        }
    }
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