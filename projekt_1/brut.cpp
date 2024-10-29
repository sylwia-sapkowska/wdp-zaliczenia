//Sylwia Sapkowska 
#include <bits/stdc++.h> 
#pragma GCC optimize("O3", "unroll-loops")
using namespace std;

vector<set<int>>s;
int Q;

void solve(){
    char c; cin >> c;
    if (c == 'C'){
        int a, q, b; cin >> a >> q >> b;
        set<int>curr;
        Q = q;
        for (int i = a; i<=b; i+=q) curr.insert(i);
        s.emplace_back(curr);
        return;
    }
    if (c == 'S'){
        int a; cin >> a;
        set<int>curr;
        curr.insert(a);
        s.emplace_back(curr);
        return;
    }
    if (c == 'U'){
        int a, b; cin >> a >> b;
        set<int>curr;
        set_union(s[a].begin(), s[a].end(), s[b].begin(), s[b].end(), inserter(curr, curr.begin()));
        s.emplace_back(curr);
        return;
    }
    if (c == 'I'){
        int a, b; cin >> a >> b;
        set<int>curr;
        set_intersection(s[a].begin(), s[a].end(), s[b].begin(), s[b].end(), inserter(curr, curr.begin()));
        s.emplace_back(curr);
        return;
    }
    if (c == 'R'){
        int a, b; cin >> a >> b;
        set<int>curr;
        set_difference(s[a].begin(), s[a].end(), s[b].begin(), s[b].end(), inserter(curr, curr.begin()));
        s.emplace_back(curr);
        return;
    }
    if (c == 'N'){
        int a, b; cin >> a >> b;
        cout << (s[a].count(b) ? 1 : 0) << "\n";
        return;
    }
    if (c == 'M'){
        int a; cin >> a;
        cout << (int)s[a].size() << "\n";
        return;
    }
    if (c == 'A'){
        int a; cin >> a;
        auto curr = s[a];
        int ans = 0;
        while ((int)curr.size()){
            int x = *curr.begin();
            curr.erase(x);
            while (curr.find(x+Q) != curr.end()) {
                curr.erase(x+Q);
                x += Q;
            }
            ans++;
        }
        cout << ans << "\n";
        return;
    }
}
 
int32_t main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int t = 1;
    s.emplace_back(set<int>());
    cin >> t; 
    while (t--) solve();

    return 0;
}